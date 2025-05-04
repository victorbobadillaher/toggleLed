#include "driver/gpio.h"
#include "FreeRTOSConfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gpio_helper.h"

void gpio_blink(int GPIO_NUM,int blink_period, int activate)
{
    if(activate)
    {
        gpio_set_level(GPIO_NUM, 1);
    } 
    else 
    {
        gpio_set_level(GPIO_NUM, 0);
    }

    vTaskDelay(pdMS_TO_TICKS(blink_period));
}

void gpio_init(int GPIO_NUM)
{
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << GPIO_NUM),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
    gpio_set_level(GPIO_NUM, 0);
}

void gpio_blink_task(void *pvParameters) {
    blink_config_t *config = (blink_config_t *)pvParameters;

    while (1) {
        gpio_set_level(config->pin, 1);
        vTaskDelay(pdMS_TO_TICKS(config->delay_ms));
        gpio_set_level(config->pin, 0);
        vTaskDelay(pdMS_TO_TICKS(config->delay_ms));
    }
}