# pragma once

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "websocket_server.h" 
#include <string.h>

static const char *TAG = "MAIN";

void wifi_init_softap();
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);