#include "wifi_manager.h"


static const char *TAG = "WIFI_MANAGER";

EventGroupHandle_t s_wifi_event_group = NULL;

esp_err_t wifi_manager_init_softapp(EventGroupHandle_t event_group_handle)
{
    //nvs and netif should already be initialized in main
    s_wifi_event_group = event_group_handle;
    if(s_wifi_event_group == NULL)
    {
        ESP_LOGI(TAG,"Event group handle is null, critical error");
        return ESP_FAIL;
    }

    // uses the api to set the wifi network interface
    esp_netif_create_default_wifi_ap();

    // calls the wifi driver for the radio antenna configurations(circuit settings)
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    
    // Chack if config was able to be set, if not program crashes
    ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));

    //register the event hanldler for the wifi events
    ESP_ERROR_CHECK(esp_event_handler_register(
        WIFI_EVENT,
        ESP_EVENT_ANY_ID,
        &wifi_manager_event_handler,
        NULL
    ));

    wifi_config_t wifi_settings_config = {
        .ap = {
            .ssid = "DUMM-E_NETWORK",
            .ssid_len = strlen("DUMM-E_NETWORK"),
            .channel = 1, // indicates the frequency its gonna send the data at
            .password = "12345678",
            .max_connection = 8,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .ssid_hidden = 0,
            .beacon_interval = 100,
        }
    };

    // if password is empty, set the auth mode to open
    if(strlen((char*) wifi_settings_config.ap.password) == 0)
    {
    
        wifi_settings_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    // set all previous config settings to the hardware
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP,&wifi_settings_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG,"SoftAP configuration started...");

    return ESP_OK;
}

void wifi_manager_event_handler(void *arg, esp_event_base_t event_base,int32_t event_id, void *event_data)
{
    if(event_base == WIFI_EVENT)
    {
        switch(event_id)
        {
            case WIFI_EVENT_AP_START:
                if(s_wifi_event_group != NULL)
                {
                    xEventGroupSetBits(s_wifi_event_group,WIFI_CONNECTED_BIT);
                }
                esp_netif_ip_info_t ip_info;

                esp_netif_t *netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

                if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
                    ESP_LOGI(TAG, "SoftAP IP Address: " IPSTR, IP2STR(&ip_info.ip));
                }

                websocket_manager_internal_start();
                ESP_LOGI(TAG, "WebSocket server instructed to start.");
                break;

            case WIFI_EVENT_AP_STACONNECTED:
                wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
                ESP_LOGI(TAG, "Station connected to SoftAP: MAC ");
                break;

            case WIFI_EVENT_AP_STADISCONNECTED:
                wifi_event_ap_stadisconnected_t* event_disconnected = (wifi_event_ap_stadisconnected_t*) event_data;
                ESP_LOGI(TAG, "Station disconnected from SoftAP: MAC ");
                break;

            default:
                break;
        }
    }

}

 void wifi_manager_task(void *pvParameters)
{
    EventGroupHandle_t wifi_event_group = (EventGroupHandle_t)pvParameters;

    if(wifi_event_group == NULL)
    {
        ESP_LOGI(TAG,"Esp wifi handle is empty: ");
        //if tasks cant run then delete
        vTaskDelete(NULL);
    }
    // set up configs in the task once
    esp_err_t err = wifi_manager_init_softapp(wifi_event_group);

    if(err != ESP_OK)
    {
        // delete the task from the stack in ram, free some space, avoid crashes
        vTaskDelete(NULL);
    }

    //in the while loop we can focus on the reconnection task
    while(1)
    {
        //add delay to avoid cpu hogging
        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

