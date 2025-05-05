#include "wifi_helper.h"

void wifi_init_softap()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&config));

    ESP_ERROR_CHECK(esp_event_handler_register(
        WIFI_EVENT,ESP_EVENT_ANY_ID,
        &wifi_event_handler, 
        NULL)
    ); 

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = "ESP32-AP",
            .ssid_len = strlen("ESP32-AP"),
            .channel = 1,
            .password = "12345678",
            .max_connection = 4,
            .authmode = WIFI_AUTH_WPA2_PSK,
        },   
    };

    if(strlen((char*)wifi_config.ap.password) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "Connect to ESP32_SERVER, pwd: 12345678");
}

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START)
    {
        websocket_server_start();
        int response = websocket_server_is_connected();

        if(websocket_server_is_connected())
        {
            ESP_LOGI(TAG, "websocket client connected");
        }
        else
        {
            ESP_LOGI(TAG, "websocket client not connected");
        }

        esp_netif_ip_info_t ip_info;
        esp_netif_t * netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

        if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            ESP_LOGI(TAG, "SoftAP IP Address: " IPSTR, IP2STR(&ip_info.ip));
        }
       ESP_LOGI(TAG, "SoftAP started");
    }
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        ESP_LOGI(TAG, "Station connected to SoftAP");
    }
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        ESP_LOGI(TAG, "Station disconnected from SoftAP");
    }
    else
    {
        ESP_LOGI(TAG, "unknown event: %ld", event_id);
    }
}