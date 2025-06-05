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

#define I2C_MASTER_TIMEOUT_MS 1000

esp_err_t i2c_master_init_bus(i2c_master_bus_handle_t *bus_handle);
esp_err_t i2c_read_register(i2c_master_bus_handle_t bus_handle, uint8_t reg_addr, uint8_t *data_out,uint16_t DEV_ADDR);
uint8_t check_address_task(void *arg);