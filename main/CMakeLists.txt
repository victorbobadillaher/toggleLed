set(SRC_FILES
    main.c
    comms/serial/i2c_helper.c
    comms/web/websocket/websocket_server.c
    hardware_manipulation/gpio/gpio_helper.c
    hardware_manipulation/wifi/wifi_helper.c
    hardware_manipulation/analog_reader.c
    sensors/sensor_helper.c
)

idf_component_register(SRCS ${SRC_FILES}
    INCLUDE_DIRS "."
        "comms/serial"
        "comms/web/websocket"
        "hardware_manipulation"
        "hardware_manipulation/gpio"
        "hardware_manipulation/wifi"
        "imu"
        "sensors"
        "monitor"
        "controls"
        "storage")

