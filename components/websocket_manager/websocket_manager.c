#include "websocket_manager.h"
#include "common_headers.h"
#include "esp_http_server,h"

static const char *TAG  = "WS_MANAGER";
static httpd_handle_t server_handle = NULL;

int client_fd = -1;


//task to handle what? 
// startup function to set up the websocket server as parent to the http_startup function y
// methods for handling incoming messages 
// methods for sending messages
// methods to verify clients connection 
// solve the multiclient problem at the end

esp_err_t websocket_manager_internal_start(void){
    //1) define http server configuration
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    //2. Start the HTTPD server
    ESP_LOGI(TAG, "starting Websocket_server...");
    esp_err_t ret = httpd_start(&server_handle,&config);

    if(ret != ESP_OK){
        ESP_LOGE(TAG, "WebSocket server already started.");
        return ret;
    }

    //3. Register the WebSocket URI handler
    httpd_uri_t ws_uri = {
        .uri = "/ws",
        .method = HTTP_GET,//what are we getting here?
        .handler  = websocket_manager_event_handler,
        .user_ctx = NULL, //user data passed to the handler, unsure on utility
        .is_websocket = true
    };

    ESP_LOGI(TAG, "Registering WebSocket URI handler %s ",ws_uri.uri);

    httpd_register_uri_handler(server_handle, &ws_uri);
    ESP_LOGI(TAG, "WebSocket server started successfully.");
    return ESP_OK;
}

static esp_err_t websocket_manager_event_handler(http_req_t *req)
{
    if(req -> method == HTTP_GET)
    {
        return handle_ws_handshake(req);
    }
    else{
        return handle_ws_data_frame(req);
    }
}

//is the task needed if the event handler can just execute functions? yes why? explain later
void websocket_manager_task(void *args)
{
    esp_err_t ret = websocket_manager_internal_start();
    if( ret != ESP_OK)
    {

    }

    while(1)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

//this method will be blocked via an event group if 2 conditions fail
static void websocket_manager_send(void *arg)
{


}


static esp_err_t handle_ws_handshake(http_req_t *req)
{
        client_fd = httpd_req_to_sockfd(req);
        ESP_LOGI(TAG,"handshake done, from FD: %d", httpd_req_to_sockfd(client_fd));
        return ESP_OK;

}

typedef enum{
    CMD_UNKNOWN = 0,
    CMD_LIGHT_ON,
    CMD_LIGHT_OFF,
    CMD_STATUS_REQUEST,
}ws_command_id_t;


static esp_err_t handle_ws_data_frame(http_rew_t *req)
{
    


}