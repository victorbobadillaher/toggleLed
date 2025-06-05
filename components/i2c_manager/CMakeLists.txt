idf_component_register(
    SRC "i2c_manager.c"
    INCLUDE_DIRS "."
    REQUIRES
        "log"
        "driver"
        "freertos"
        "esp_system"
        "esp_event"
        "nvs_flash"
)