# components/wifi_manager/CMakeLists.txt

idf_component_register(
    SRCS "wifi_manager.c"
    INCLUDE_DIRS "." 
    REQUIRES
        "esp_wifi"      
        "esp_event"     
        "esp_netif"     
        "freertos"      
        "nvs_flash"     
        "main"
    PRIV_REQUIRES
        "esp_log"       # Used internally, but not exposed to other components
)