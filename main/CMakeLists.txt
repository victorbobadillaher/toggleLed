
set(SRC_FILES
    main.c
    main1.c
    analog_reader.c
    gpio_helper.c
    wifi_helper.c
    websocket_server.c
)

idf_component_register(
    SRCS ${SRC_FILES}
    INCLUDE_DIRS "."
)
