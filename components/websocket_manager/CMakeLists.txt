idf_component_register(
    SRCS "websocket_manager.c"
    INCLUDE_DIRS "."
    REQUIRES 
        "esp_log"       # Logging library
        "esp_event"     # Event handling library
        "freertos"      # FreeRTOS support
        "nvs_flash"     # Non-volatile storage
        "esp_websocket_client"  # WebSocket client library
        "esp_http_server"     # HTTP server library
        "main"
)