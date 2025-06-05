set(SRC_FILES
    imu_math.c
    imu_task.c
    imu_driver.c
)

idf_component_register(
    SRCS  ${SRC_FILES}
    INCLUDE_DIRS "."
    REQUIRES
        "esp_event"    
        "freertos"
        "nvs_flash"
        "log" 
        "esp_system"
        "driver"
        "i2c_manager"
        "esp_common"
        )
