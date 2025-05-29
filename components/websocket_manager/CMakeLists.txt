# components/websocket_manager/CMakeLists.txt
# This CMakeLists.txt defines the 'websocket_manager' component.

idf_component_register(
    SRCS "websocket_manager.c"
    INCLUDE_DIRS "."
    REQUIRES
        "log"               # For ESP_LOGI, ESP_LOGE macros
        "esp_http_server"   # For httpd_config_t, httpd_uri_t, etc.
        # Note: esp_http_client is for WEBSOCKET CLIENT. For SERVER, esp_http_server is key.
        # If you were making a WebSocket CLIENT, you'd need esp_websocket_client.
        # However, esp_http_server uses underlying network functions that may pull in parts of http_client/lwip.
        "freertos"          # For vTaskDelay, pdMS_TO_TICKS, xTaskCreate
        "esp_system"        # For general system functions like esp_err_t, ESP_OK, esp_err_to_name
        "lwip"              # Underlying TCP/IP stack; often implicitly brought in by others
        "cjson"             # <--- Make sure this is present for JSON operations
        "esp_wifi"
        "nvs_flash"
        "esp_event"
)