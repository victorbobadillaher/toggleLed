#include "i2c_helper.h"
#include "common_headers.h"

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
<
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