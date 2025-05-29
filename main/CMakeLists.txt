idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        "esp_wifi"
        "esp_event"    
        "esp_netif"
        "freertos"
        "nvs_flash"
        "log" 
        "wifi_manager"
        "websocket_manager"
)

