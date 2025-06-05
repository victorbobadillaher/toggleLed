// Custom includes
#include "includes.h"


#include "wifi_manager.h"
#include "websocket_manager.h"
#include "i2c_manager.h"

#include "imu_driver.h"
#include "imu_math.h"
#include "imu_types.h"
#include "imu_task.h"

#include "gpio_helper.h"


static const char* TAG = "MAIN";

#define WIFI_CONNECTED_BIT BIT0

#define BLINKER_GPIO 2

static EventGroupHandle_t s_wifi_event_group = NULL;

void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    i2c_master_bus_handle_t bus_handle;
    i2c_master_init_bus(&bus_handle);

    esp_log_level_set("*", ESP_LOG_INFO);

    s_wifi_event_group = xEventGroupCreate();
    if (s_wifi_event_group == NULL) {
        ESP_LOGE(TAG, "Failed to create WiFi event group");
        return;
    }
    
    xTaskCreatePinnedToCore(wifi_manager_task,
         "wifi_manager_task",
          4096, (void *)s_wifi_event_group,
           5,
        NULL,
         0);

    imu_config_t imu_cfg = {
        .bus = bus_handle,
        .dev_address = 0x68,
        .sample_rate_hz = 100,
        .accel_range = 2,
        .gyro_range = 250,
        .filter_bandwidth = 20
    };

    xTaskCreatePinnedToCore(
        imu_task,
        "imu_task",
        4096,               // Stack size
        &imu_cfg,               // Params
        7,                 // Priority
        NULL,               // Task handle
        1                   // Core
    );


    gpio_init(BLINKER_GPIO);
    xTaskCreatePinnedToCore(

        gpio_blink_task,
        "gpio_blink_task",
        2048,               // Stack size
        &(blink_config_t){.pin = BLINKER_GPIO, .delay_ms = 100}, // Params
        2,                 // Priority
        NULL,               // Task handle
        1                   // Core
    );

    while(1)
    {
        printf("Hello, World_ new one!\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

