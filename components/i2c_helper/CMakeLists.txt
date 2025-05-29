idf_component_registers(
    SRC "i2c_helper.c"
    INCLUDE_DIRS "."
    REQUIRES
        "log"
        "driver"
        "freertos"
        "esp_system"
        "esp_err"
        "esp_event"
        "nvs_flash"
        "main"
)