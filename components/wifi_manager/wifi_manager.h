#pragma once

#include <string.h>
#include <stdlib.h> 
#include <stdint.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_wifi.h"
#include "esp_mac.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_err.h"
#include "esp_event_base.h"
#include "sdkconfig.h"
#include "esp_netif.h"
#include "esp_system.h"
#include "string.h"
#include "esp_wifi_types.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_netif.h"
#include "websocket_manager.h"

#define WIFI_CONNECTED_BIT BIT0 // Bit for event group to indicate wifi connection status

void wifi_manager_task(void *pvParameters);

esp_err_t wifi_manager_init_softapp(EventGroupHandle_t event_group_handle);

void wifi_manager_event_handler(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data);