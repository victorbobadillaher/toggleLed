/*#include <stdio.h>
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include <string.h>
#include "driver/gpio.h"
#include "gpio_helper.h"
#include "i2c_helper.h"
#include "wifi_helper.h"

static const char *TAG = "MAIN";

#define LED_GPIO_PIN 2

void wifi_init_task(void *pvParameters)
{
    wifi_init_softap();
    vTaskDelete(NULL);
}

void i2c_task_main(void * arg) {
    i2c_master_init();
    check_address_task(arg);
} 

void app_main(void)
{
    gpio_init(LED_GPIO_PIN);

    esp_log_level_set("*", ESP_LOG_INFO); 
    xTaskCreate(wifi_init_task, "wifi_init_task", 4096, NULL, 5, NULL);
    //xTaskCreate(i2c_task_main, "i2c_task", 4096, NULL, 5, NULL);

    while (1)
    {
        gpio_blink(LED_GPIO_PIN,100,1);
        gpio_blink(LED_GPIO_PIN,100,0);
    }
}*/