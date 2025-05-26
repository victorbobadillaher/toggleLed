#pragma once

#include "esp_err.h"
#include "freertos/event_groups.h"

#define WIFI_CONNECTED_BIT BIT0 // Bit for event group to indicate wifi connection status

static void wifi_manager_task(void *pvParameters);

esp_err_t wifi_manager_init_softapp(EventGroupHandle_t event_group_handle);

static void wifi_manager_event_handler(void *arg, event_base_t event_base,int32 event_id, void *event_data);