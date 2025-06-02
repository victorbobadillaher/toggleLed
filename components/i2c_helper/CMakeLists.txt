idf_component_register(
    SRC "i2c_helper.c"
    INCLUDE_DIRS "."
    REQUIRES
        "log"
        "driver"
        "freertos"
        "esp_system"
        "esp_event"
        "nvs_flash"
        "main"
)