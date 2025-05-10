#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "gpio_helper.h"
#include "wifi_helper.h"
//#include "esp_adc/adc_oneshot.h"

#define I2C_MASTER_TIMEOUT_MS 1000
#define TAG "I2C_SCANNER"

#define LED_GPIO 2

TaskHandle_t CheckAddressHandle = NULL;

// Function for initializing I2C bus
static void i2c_master_init_bus(i2c_master_bus_handle_t *bus_handle)
{
    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C_NUM_0,
        .sda_io_num = GPIO_NUM_21,
        .scl_io_num = GPIO_NUM_22,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
    ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, bus_handle));
}

// Task to scan all I2C addresses
void check_address_task(void *arg)
{
    i2c_master_bus_handle_t bus_handle = (i2c_master_bus_handle_t)arg;
    while (1)
    {
        for (uint8_t addr = 0x03; addr < 0x78; addr++)
        {
            esp_err_t err = i2c_master_probe(bus_handle, addr, I2C_MASTER_TIMEOUT_MS);
            if (err == ESP_OK)
            {
                ESP_LOGI(TAG, "Found I2C device at address: 0x%02X", addr);
            }
        }
        ESP_LOGI(TAG, "I2C scan complete");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

esp_err_t i2c_read_register(i2c_master_bus_handle_t bus_handle, uint8_t reg_addr, uint8_t *data_out,uint16_t DEV_ADDR)
{
    i2c_master_dev_handle_t dev_handle;
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = DEV_ADDR,
        .scl_speed_hz = 100000,
    };

    // Attach device to bus (temporary handle)
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    // Write the register address we want to read
    ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, &reg_addr, 1, -1));

    // Read the data into data_out
    ESP_ERROR_CHECK(i2c_master_receive(dev_handle, data_out, 1, -1));

    // Clean up the handle if you don't plan to reuse it
    ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));

    return ESP_OK;
}


void mpu6050_read_task(void *params)
{
    struct mpu_task_params {
        i2c_master_bus_handle_t bus;
        uint8_t dev_addr;
        uint16_t sample_rate_hz;
    };

    struct mpu_task_params *cfg = (struct mpu_task_params *)params;

    const uint8_t ACCEL_START_REG = 0x3B;
    uint8_t raw_data[6];
    int16_t accel_x, accel_y, accel_z;

    TickType_t delay_ticks = pdMS_TO_TICKS(1000 / cfg->sample_rate_hz);

    while (1)
    {
        i2c_master_dev_handle_t dev_handle;

        i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = cfg->dev_addr,
            .scl_speed_hz = 100000,
        };

        ESP_ERROR_CHECK(i2c_master_bus_add_device(cfg->bus, &dev_cfg, &dev_handle));

        // Tell the sensor we want to read from 0x3B
        ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, &ACCEL_START_REG, 1, -1));

        // Read 6 bytes: XH, XL, YH, YL, ZH, ZL
        ESP_ERROR_CHECK(i2c_master_receive(dev_handle, raw_data, 6, -1));

        // Parse 16-bit signed values
        accel_x = (raw_data[0] << 8) | raw_data[1];
        accel_y = (raw_data[2] << 8) | raw_data[3];
        accel_z = (raw_data[4] << 8) | raw_data[5];

        printf("Accel X: %d\tY: %d\tZ: %d\n", accel_x, accel_y, accel_z);

        ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));

        vTaskDelay(delay_ticks);
    }
}

void wifi_init_task(void *pvParameters)
{
    wifi_init_softap();
    vTaskDelete(NULL);
}

void app_main(void)
{

    esp_log_level_set("*", ESP_LOG_INFO); 
    xTaskCreate(wifi_init_task, "wifi_init_task", 4096, NULL, 5, NULL);

    i2c_master_bus_handle_t bus_handle;
    i2c_master_init_bus(&bus_handle);

    xTaskCreatePinnedToCore(check_address_task, "Scan I2C", 4096, (void *)bus_handle, 10, &CheckAddressHandle, 1);
    
    static struct mpu_task_params {
        i2c_master_bus_handle_t bus;
        uint8_t dev_addr;
        uint16_t sample_rate_hz;
    } mpu_cfg;
    
    mpu_cfg.bus = bus_handle;
    mpu_cfg.dev_addr = 0x68;  // your device address
    mpu_cfg.sample_rate_hz = 100;

    xTaskCreatePinnedToCore(mpu6050_read_task, "MPU6050 Reader", 4096, &mpu_cfg, 5, NULL, 1);

    gpio_init(LED_GPIO);

    while(1)
    {
        gpio_blink(LED_GPIO, 100, 1);
        gpio_blink(LED_GPIO, 100, 0);
    }
}
