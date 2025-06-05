#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

// ESP-IDF common includes
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_http_server.h"
#include "lwip/ip_addr.h"
#include "sdkconfig.h"

// Custom includes

//commnications includes
#include "driver/i2c_master.h"

#include "wifi_manager.h"
#include "websocket_manager.h"


static const char* TAG = "MAIN";

# define WIFI_CONNECTED_BIT BIT0

static EventGroupHandle_t s_wifi_event_group = NULL;



void app_main(void)
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

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

    while(1)
    {
        printf("Hello, World_ new one!\n");
        vTaskDelay(pdMS_TO_TICKS(1000)); // Delay for 1 second
    }
}

