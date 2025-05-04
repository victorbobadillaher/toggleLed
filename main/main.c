#include <stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "websocket_server.h" 
#include <string.h>
#include "driver/gpio.h"
#include "gpio_helper.h"
#include "i2c_helper.h"

static const char *TAG = "MAIN";

#define LED_GPIO_PIN 2
#define i2c_GPIO_SDA 21
#define i2c_GPIO_SCL 22

void wifi_init_softap();
void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

void wifi_init_task(void *pvParameters)
{
    printf("initiating wifi task\n");
    wifi_init_softap();
    vTaskDelete(NULL);
}

void i2c_task(void *pvParameters)
{
    printf("initiating i2c task\n");
    i2c_master_init();

    while(1){
        //read_who_am_i();
        send_start_only();
        vTaskDelay(pdMS_TO_TICKS(500));
    }
    //vTaskDelete(NULL);
}

void app_main(void)
{
    gpio_init(LED_GPIO_PIN);

    /*gpio_init(i2c_GPIO_SDA);
    gpio_init(i2c_GPIO_SCL);
    static blink_config_t scl_config = { .pin = i2c_GPIO_SCL, .delay_ms = 200 };
    static blink_config_t sda_config = { .pin = i2c_GPIO_SDA, .delay_ms = 100 };
    xTaskCreate(gpio_blink_task,"blink_scl",2048,&scl_config,5,NULL);
    xTaskCreate(gpio_blink_task,"blink_sda",2048,&sda_config,5,NULL);*/

    esp_log_level_set("*", ESP_LOG_INFO); 
    xTaskCreate(wifi_init_task, "wifi_init_task", 4096, NULL, 5, NULL);

    xTaskCreate(i2c_task, "i2c_task", 2048, NULL, 5, NULL);
    
    while (1)
    {
        gpio_blink(LED_GPIO_PIN,100,1);
        gpio_blink(LED_GPIO_PIN,100,0);
    }
}


void wifi_init_softap()
{

    printf("Entering soft ap\n");

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

    printf("registering event handler\n");

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
        printf("if no pwd if\n");
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config)); // sets ap configs
    ESP_ERROR_CHECK(esp_wifi_start()); //starts the wifi network

    ESP_LOGI(TAG, "Connect to ESP32_SERVER, pwd: 123456789");
}

void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    printf("entering the wifi event handler\n");
    if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START)
    {
        websocket_server_start();
        printf("websocket server started\n");

        int response = websocket_server_is_connected();

        printf("WS connected?: %d\n", response);

        if(websocket_server_is_connected())
        {
            //printf("websocket client id connected\n");
            ESP_LOGI(TAG, "websocket client connected");
        }

        
        else
        {
            //printf("websocket client not connected\n");
            ESP_LOGI(TAG, "websocket client not connected");
        }


        esp_netif_ip_info_t ip_info;
        esp_netif_t * netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

        if (netif && esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
            //printf("nET IF IS OK i am in the eifi event handler\n");

            ESP_LOGI(TAG, "SoftAP IP Address: " IPSTR, IP2STR(&ip_info.ip));
        }
       ESP_LOGI(TAG, "SoftAP started");
    }
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        //printf("connected \n");
        ESP_LOGI(TAG, "Station connected to SoftAP");
    }
    else if(event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        //printf("disconnected\n");
        ESP_LOGI(TAG, "Station disconnected from SoftAP");
    }

    else
    {
        printf("unkwonwn event \n");
        //ESP_LOGI(TAG, "unknown event: %ld", event_id);
    }
}