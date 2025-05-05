idf_component_register(SRCS "main.c" 
                "websocket_server.c"
                "gpio_helper.c"
                "i2c_helper.c"
                "wifi_helper.c"
                    INCLUDE_DIRS "."
                    REQUIRES esp_driver_gpio 
                    esp_driver_i2c
                    esp_wifi
                    nvs_flash
                    esp_event
                    esp_http_server
                    esp_http_client
                    )
