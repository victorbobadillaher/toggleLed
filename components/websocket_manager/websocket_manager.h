#pragma once

#include <string.h>
#include <stdlib.h> 
#include <stdint.h>

#include "esp_http_server.h" 
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

// Enum for WebSocket commands (can be moved here if not truly common elsewhere)
typedef enum {
    CMD_UNKNOWN = 0,
    CMD_LIGHT_ON,
    CMD_LIGHT_OFF,
    CMD_STATUS_REQUEST,
    // Add more commands as needed
} ws_command_id_t;

/**
 * @brief Initializes and starts the WebSocket server.
 *
 * @return ESP_OK on success, error code otherwise.
 */
esp_err_t websocket_manager_internal_start(void);

/**
 * @brief Task for the WebSocket manager.
 * This task will typically initialize the server and then handle
 * background operations like periodic sending or event waiting.
 *
 * @param args Arguments passed to the task.
 */
void websocket_manager_task(void *args);

/**
 * @brief Sends a text message over WebSocket to the currently connected client.
 *
 * @param message The null-terminated string message to send.
 * @return ESP_OK on success, error code otherwise.
 */
esp_err_t websocket_manager_send_text(const char *message);
