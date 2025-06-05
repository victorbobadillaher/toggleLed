set(SRC_FILES
    imu_driver.c
    imu_math.c
    imu_task.c
)

idf_component_register(
    SRCS  ${SRC_FILES}
    INCLUDE_DIRS "."
    REQUIRES  
        "esp_system" 
        "esp_common"
        "log"
        "driver"
        "i2c_manager"
        "websocket_manager"
        )


        