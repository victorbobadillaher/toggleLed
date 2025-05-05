#pragma once

#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"
#include<freertos/FreeRTOS.h>
#include<freertos/task.h>
#include "esp_log.h"


#define I2C_MASTER_SCL_IO           22  
#define I2C_MASTER_SDA_IO           21
#define I2C_MASTER_NUM              I2C_NUM_0
#define I2C_MASTER_FREQ_HZ          100000
#define I2C_MASTER_TX_BUF_DISABLE   0
#define I2C_MASTER_RX_BUF_DISABLE   0

void i2c_master_init(void);
void i2c_send_test_transaction(void);
void i2c_task_main(void *pvParameters);
void check_address_task(void *arg);