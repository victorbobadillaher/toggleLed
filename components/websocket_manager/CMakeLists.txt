idf_component_register(
    SRCS "websocket_manager.c"
    INCLUDE_DIRS "."
    REQUIRES
        "log"
        "esp_http_server"
        "esp_http_client"
        "freertos"
        "esp_system"
        "lwip"
        "esp_wifi"
        "nvs_flash"
        "esp_event"
        "esp_netif"
        "esp_common")