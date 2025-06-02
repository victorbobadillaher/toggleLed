#pragma once

#include "freertos/event_groups.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "esp_mac.h"

#define WIFI_CONNECTED_BIT BIT0 // Bit for event group to indicate wifi connection status

static void wifi_manager_task(void *pvParameters);

esp_err_t wifi_manager_init_softapp(EventGroupHandle_t event_group_handle);

static void wifi_manager_event_handler(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data);