#pragma once

typedef enum{
    CMD_UNKNOWN = 0,
    CMD_LIGHT_ON,
    CMD_LIGHT_OFF,
    CMD_STATUS_REQUEST,
    CMD_GET_DEVICE_INFO,
    //Imu configs
    WS_CMD_START_IMU_STREAM,
    WS_CMD_STOP_IMU_STREAM,
    WS_CMD_GET_IMU_CONFIG,
    WS_CMD_SET_IMU_CONFIG
}ws_command_id_t;

//build struct to hold commands and context (which client sent the command)
typedef struct{
    ws_command_id_t command_id; //command id
    int client_fd; //client file descriptor
    void *context; //context for the command, can be used to pass additional data
}ws_command_t;

// Public API functions for the WebSocket Manager component
esp_err_t websocket_manager_init(void); // For initializing queues, mutexes etc.
void websocket_manager_task(void *pvParameters); // The main RTOS task
void websocket_manager_send(const char *message); // Send to currently connected client(s)
int websocket_manager_is_connected(void); // Check if any client is connected