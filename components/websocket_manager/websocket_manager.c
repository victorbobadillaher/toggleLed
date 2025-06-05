#include "websocket_manager.h"


static const char *TAG = "WS_MANAGER";
static httpd_handle_t server_handle = NULL;
static int client_fd = -1; 

static esp_err_t websocket_manager_event_handler(httpd_req_t *req);
static esp_err_t handle_ws_handshake(httpd_req_t *req);
static esp_err_t handle_ws_data_frame(httpd_req_t *req);
static ws_command_id_t map_ws_command(char *cmd_str);

esp_err_t websocket_manager_internal_start(void) {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    // config.server_port = 80;

    ESP_LOGI(TAG, "Starting WebSocket server...");
    esp_err_t ret = httpd_start(&server_handle, &config);

    if (ret != ESP_OK) {
        if (ret == ESP_ERR_HTTPD_RESP_HDR) {
            ESP_LOGE(TAG, "WebSocket server failed to start: Socket error (e.g., port in use). Error: %s", esp_err_to_name(ret));
        } else {
            ESP_LOGE(TAG, "WebSocket server failed to start with error: %s", esp_err_to_name(ret));
        }
        return ret;
    }

    httpd_uri_t ws_uri = {
        .uri        = "/ws",
        .method     = HTTP_GET,
        .handler    = websocket_manager_event_handler,
        .user_ctx   = NULL, // User data passed to the handler
        .is_websocket = true
    };

    ESP_LOGI(TAG, "Registering WebSocket URI handler %s", ws_uri.uri);
    httpd_register_uri_handler(server_handle, &ws_uri);
    ESP_LOGI(TAG, "WebSocket server started successfully.");
    return ESP_OK;
}

void websocket_manager_task(void *args) {
    esp_err_t ret = websocket_manager_internal_start();

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start WebSocket manager task due to server error. Deleting task.");
        vTaskDelete(NULL);
        return;
    }

    // Example: send a message every 5 seconds if a client is connected
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = pdMS_TO_TICKS(5000); // 5000ms = 5 seconds
    xLastWakeTime = xTaskGetTickCount(); // Initialize xLastWakeTime with the current tick count

    while (1) {
        vTaskDelayUntil(&xLastWakeTime, xFrequency);

        if (client_fd != -1) {
            ESP_LOGI(TAG, "Sending periodic message to client FD: %d", client_fd);
            websocket_manager_send_text("Hello from ESP32 (periodic)!");
        }
    }
}

esp_err_t websocket_manager_send_text(const char *message) {
    if (client_fd == -1 || server_handle == NULL) {
        ESP_LOGW(TAG, "Cannot send: No WebSocket client connected or server not running.");
        return ESP_FAIL;
    }

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t)); // Initialize struct to zeros
    ws_pkt.payload = (uint8_t *)message;          // Cast const char* to uint8_t* for payload
    ws_pkt.len = strlen((char *)ws_pkt.payload);  // Get length of the message
    ws_pkt.type = HTTPD_WS_TYPE_TEXT;             // Specify it's a text frame

    esp_err_t send_ret = httpd_ws_send_frame_async(server_handle, client_fd, &ws_pkt);
    if (send_ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to send WebSocket text frame: %s", esp_err_to_name(send_ret));
    } else {
        ESP_LOGI(TAG, "Sent text message: '%s' to FD: %d", message, client_fd);
    }
    return send_ret;
}

// --- Static (Internal) Functions ---

static esp_err_t websocket_manager_event_handler(httpd_req_t *req) {
    // Check if it's an initial GET request (handshake) or a WebSocket data frame
    if (req->method == HTTP_GET) {
        // This is the initial WebSocket handshake request
        return handle_ws_handshake(req);
    }

    // If it's not a GET, and it's a WebSocket-enabled URI, it must be a WebSocket data frame.
    // The httpd_ws_recv_frame handles data or control frames (like PING, PONG, CLOSE).
    return handle_ws_data_frame(req);
}

static esp_err_t handle_ws_handshake(httpd_req_t *req) {
    client_fd = httpd_req_to_sockfd(req); // Get the file descriptor of the new client
    // Corrected logging: client_fd already holds the socket descriptor
    ESP_LOGI(TAG, "WebSocket handshake done, client FD: %d", client_fd);
    return ESP_OK;
}

static ws_command_id_t map_ws_command(char *cmd_str) {
    if (cmd_str == NULL) return CMD_UNKNOWN;
    if (strcmp(cmd_str, "on") == 0) return CMD_LIGHT_ON;
    if (strcmp(cmd_str, "off") == 0) return CMD_LIGHT_OFF;
    if (strcmp(cmd_str, "status") == 0) return CMD_STATUS_REQUEST;
    return CMD_UNKNOWN;
}

static esp_err_t handle_ws_data_frame(httpd_req_t *req) {
    // Ensure the request is indeed a WebSocket request
    if (httpd_ws_get_fd_info(req,1) == -1) {
        ESP_LOGE(TAG, "Received non-WebSocket request on WebSocket handler.");
        return ESP_FAIL;
    }

    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t)); // Initialize frame structure
    ws_pkt.payload = NULL; // Crucial for first call to httpd_ws_recv_frame to get length
    ws_pkt.len = 0;

    // First call to httpd_ws_recv_frame to get the length of the incoming frame
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt,0);

    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to get frame length: %s", esp_err_to_name(ret));
        if (httpd_ws_get_fd_info(req,1) == -1) {
            ESP_LOGI(TAG, "WebSocket client closed connection, FD: %d", client_fd);
            client_fd = -1; // Invalidate client FD
        }
        return ret;
    }

    // If payload_len is 0, it might be a control frame (e.g., PING/PONG, CLOSE) or an empty data frame
    if (ws_pkt.len == 0) {
        ESP_LOGI(TAG, "Received empty WebSocket frame or control frame (type: %d).", ws_pkt.type);
        // Specifically handle client close frame to update client_fd
        if (ws_pkt.type == HTTPD_WS_TYPE_CLOSE) {
             ESP_LOGI(TAG, "WebSocket client explicitly sent close frame for FD: %d", client_fd);
             client_fd = -1; // Invalidate client FD
        }
        return ESP_OK; // No data to process
    }

    // Allocate a buffer to hold the received data + 1 for null terminator
    // IMPORTANT: Make sure this buffer is large enough for your expected max message size
    // You might want to use a fixed-size buffer or dynamic allocation based on 'ws_pkt.len'
    uint8_t *buf = (uint8_t *)calloc(1, ws_pkt.len + 1); // +1 for null terminator
    if (buf == NULL) {
        ESP_LOGE(TAG, "Failed to allocate memory for WebSocket frame buffer");
        return ESP_ERR_NO_MEM;
    }
    ws_pkt.payload = buf; // Set payload pointer to our allocated buffer

    ret = httpd_ws_recv_frame(req, &ws_pkt,0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "httpd_ws_recv_frame failed to read frame data: %s", esp_err_to_name(ret));
        free(buf);
        return ret;
    }

    buf[ws_pkt.len] = '\0';
    // Only process text data frames here for simplicity
    if (ws_pkt.type == HTTPD_WS_TYPE_TEXT) {
        ESP_LOGI(TAG, "Received WebSocket text data (FD: %d): '%s'", client_fd, (char *)buf);

        ws_command_id_t cmd = map_ws_command((char *)buf);

        switch (cmd) {
            case CMD_UNKNOWN:
                ESP_LOGW(TAG, "Unknown command received: %s", buf);
                websocket_manager_send_text("{\"error\":\"Unknown command\"}");
                break;
            case CMD_LIGHT_ON:
                ESP_LOGI(TAG, "Received CMD_LIGHT_ON. Turning light ON.");
                // TODO: Implement actual light ON logic here
                // (e.g., call a GPIO helper function)
                websocket_manager_send_text("{\"status\":\"Light ON\"}");
                break;
            case CMD_LIGHT_OFF:
                ESP_LOGI(TAG, "Received CMD_LIGHT_OFF. Turning light OFF.");
                websocket_manager_send_text("{\"status\":\"Light OFF\"}");
                break;
            case CMD_STATUS_REQUEST:
                ESP_LOGI(TAG, "Received CMD_STATUS_REQUEST. Sending status.");
                break;
            default:
                break; // Should not happen with CMD_UNKNOWN catch
        }
    } else {
        // Handle other frame types if needed (e.g., binary, close, ping, pong)
        ESP_LOGI(TAG, "Received non-text WebSocket frame (type: %d, len: %d)", ws_pkt.type, ws_pkt.len);
        // You might want to respond to PING with PONG, or handle binary data.
    }

    free(buf);
    return ESP_OK;
}