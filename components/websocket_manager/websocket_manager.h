#pragma once

#include <string.h>
#include <stdlib.h> 
#include <stdint.h>

#include "esp_http_server.h" 
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_event.h"
#include "sdkconfig.h"
#include "esp_http_server.h"
#include "esp_http_client.h"
#include "esp_err.h"
#include "esp_netif.h"
#include "esp_err.h"
#include "esp_event_base.h"




// Enum for WebSocket commands (can be moved here if not truly common elsewhere)
typedef enum {
    CMD_UNKNOWN = 0,
    CMD_LIGHT_ON,
    CMD_LIGHT_OFF,
    CMD_STATUS_REQUEST,
    // Add more commands as needed
} ws_command_id_t;

esp_err_t websocket_manager_internal_start(void);

void websocket_manager_task(void *args);

esp_err_t websocket_manager_send_text(const char *message);

int websocket_server_is_connected(void);