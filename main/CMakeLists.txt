idf_component_register(
    SRCS  "main.c" "gpio_helper.c"
    INCLUDE_DIRS "."
    REQUIRES
        "esp_wifi"
        "esp_event"    
        "esp_netif"
        "freertos"
        "nvs_flash"
        "log" 
        "esp_http_server"
        "esp_http_client"
        "esp_system"
        "lwip"
        "driver"
        "wifi_manager"
        "websocket_manager"
        "i2c_manager"
        "imu_manager"
        )

