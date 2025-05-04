#include "i2c_helper.h"
#include "esp_log.h"

#define I2C_MASTER_NUM      I2C_NUM_0
#define I2C_MASTER_SCL_IO   22
#define I2C_MASTER_SDA_IO   21
#define I2C_MASTER_FREQ_HZ  100000  // Standard 100kHz I2C

void i2c_master_init(void){

    i2c_config_t conf ={
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    esp_err_t err;

    err = i2c_param_config(I2C_MASTER_NUM, &conf);
    if (err != ESP_OK) ESP_LOGE("I2C", "Param config failed: %s", esp_err_to_name(err));

    err = i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);
    if (err != ESP_OK) ESP_LOGE("I2C", "Driver install failed: %s", esp_err_to_name(err));
}

void send_start_only() {
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    i2c_master_start(cmd);                         // START condition
    i2c_master_write_byte(cmd, 0x68 << 1, true);   // MPU6050 or dummy addr
    i2c_master_stop(cmd);                          // STOP condition

    //i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));

    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    if (ret == ESP_OK) {
        ESP_LOGI("I2C", "Device at 0x68 ACKed");
    } else {
        ESP_LOGE("I2C", "No ACK from 0x68: %s", esp_err_to_name(ret));
    }


    i2c_cmd_link_delete(cmd);
}

void read_who_am_i() {
    uint8_t who_am_i = 0;
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();

    // START and write address + register (0x75)
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x68 << 1) | I2C_MASTER_WRITE, true); // Write mode
    i2c_master_write_byte(cmd, 0x75, true); // WHO_AM_I register
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Write phase failed: %s", esp_err_to_name(ret));
        return;
    }

    // Now read 1 byte from the register
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (0x68 << 1) | I2C_MASTER_READ, true); // Read mode
    i2c_master_read_byte(cmd, &who_am_i, I2C_MASTER_NACK); // Read 1 byte, then NACK
    i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, pdMS_TO_TICKS(1000));
    i2c_cmd_link_delete(cmd);
    if (ret != ESP_OK) {
        ESP_LOGE("I2C", "Read phase failed: %s", esp_err_to_name(ret));
        return;
    }

    ESP_LOGE("I2C", "WHO_AM_I = 0x%02X", who_am_i);
}
