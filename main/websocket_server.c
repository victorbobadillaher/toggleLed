#include "esp_log.h"
#include "esp_http_server.h"
#include "gpio_helper.h"

static const char *TAG = "WS_SERVER";

static httpd_handle_t server_handle = NULL;
static int client_fd = -1; // File descriptor for the connected client

#define GPIO_NUM 23 // GPIO pin number for the LED

// Forward declaration
static esp_err_t websocket_handler(httpd_req_t *req);
int websocket_server_is_connected(void);
void websocket_server_send(const char *message);

void websocket_server_start(void)
{
     httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    esp_err_t ret = httpd_start(&server_handle, &config);
    
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to start HTTP server: %s", esp_err_to_name(ret));
        return;
    }

    httpd_uri_t ws_uri = {
        .uri = "/ws",
        .method = HTTP_GET,
        .handler = websocket_handler,
        .user_ctx = NULL,
        .is_websocket = true
    };

    httpd_register_uri_handler(server_handle, &ws_uri);
}

int websocket_server_is_connected(void)
{
    return client_fd != -1;
}

void websocket_server_send(const char *message)
{
    if (client_fd == -1)
    {
        ESP_LOGW(TAG, "No client connected to send message");
        return;
    }

    httpd_ws_frame_t ws_pkt = {
        .type = HTTPD_WS_TYPE_TEXT,
        .final = true,
        .payload = (uint8_t *)message,
        .len = strlen(message),
    };

    esp_err_t ret = httpd_ws_send_frame_async(server_handle, client_fd, &ws_pkt);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to send WebSocket frame: %s", esp_err_to_name(ret));
    }
}

static esp_err_t websocket_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "📥 WebSocket request received");
    ESP_LOGI(TAG, "🧭 URI: %s", req->uri);
    ESP_LOGI(TAG, "🛠 Method: %d", req->method);

    if (req->method == HTTP_GET)
    {
        printf("Handshake done, new WebSocket connection\n");
        ESP_LOGI(TAG, "Handshake done, new WebSocket connection");
        client_fd = httpd_req_to_sockfd(req);
        return ESP_OK;
    }

    // Not a GET -> it’s a WebSocket data frame
    httpd_ws_frame_t ws_pkt;
    memset(&ws_pkt, 0, sizeof(httpd_ws_frame_t)); // clear the struct

    // First, get the frame size
    esp_err_t ret = httpd_ws_recv_frame(req, &ws_pkt, 0);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to get WebSocket frame size: %s", esp_err_to_name(ret));
        return ret;
    }

    char *buf = NULL;

    if (ws_pkt.len)
    {
        buf = malloc(ws_pkt.len + 1);
        if (buf == NULL)
        {
            ESP_LOGE(TAG, "Failed to allocate memory for WebSocket frame payload");
            return ESP_FAIL;
        }

        ws_pkt.payload = (uint8_t *)buf; // Set the payload to the allocated buffer

        ret = httpd_ws_recv_frame(req, &ws_pkt, ws_pkt.len);
        if (ret != ESP_OK)
        {
            ESP_LOGE(TAG, "Failed to read WebSocket frame: %s", esp_err_to_name(ret));
            free(buf);
            return ret;
        }

        buf[ws_pkt.len] = '\0';  // Null-terminate

        if (ws_pkt.len == 2 && strncmp(buf, "on", 2) == 0)
        {
            websocket_server_send("Esp says: ✅ Button was clicked!");
            gpio_init(GPIO_NUM);
            gpio_blink(GPIO_NUM,1000,1);
            gpio_blink(GPIO_NUM,1000,0);
        }

        ESP_LOGI(TAG, "Received message: %s", buf);
        free(buf);
    }
    return ESP_OK;
}
