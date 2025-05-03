#pragma once //tells the compiler to not duplicate this functions 

// Function to start the server
void websocket_server_start(void);

// Function to send message to the client
void websocket_server_send(const char *data);

// Function to check if a client is connected
int websocket_server_is_connected(void);