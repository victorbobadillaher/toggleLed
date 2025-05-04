#pragma once
#include "driver/gpio.h"

typedef struct{
    gpio_num_t pin;
    int delay_ms;
}blink_config_t;

void gpio_blink(int GPIO_NUM,int blink_period, int activate);

void gpio_init(int GPIO_NUM);

void gpio_blink_task(void *pvParameters);

void read_who_am_i();
