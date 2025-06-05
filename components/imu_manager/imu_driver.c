#include "imu_driver.h"
#include <stdint.h>

static const char *TAG = "IMU_DRIVER";

static i2c_master_dev_handle_t imu_dev_handle = NULL;

esp_err_t imu_driver_init(imu_config_t imu_cfg)
{

    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = imu_cfg.dev_address,
        .scl_speed_hz = 100000, // 100kHz
        .flags.disable_ack_check = false, // Enable ACK check
    };
    
    return ESP_OK;
}

esp_err_t imu_driver_read(imu_data_t * out_data, imu_config_t * imu_cfg)
{
    const uint8_t ACCEL_REG = 0x3B;
    uint8_t accel_buf[6];
    ESP_ERROR_CHECK(i2c_read_register(imu_cfg -> bus, ACCEL_REG, accel_buf, imu_cfg -> dev_address));

    out_data -> acceleration.x = (int16_t) (accel_buf[0] << 8 | accel_buf[1]);
    out_data -> acceleration.y = (int16_t) (accel_buf[2] << 8 | accel_buf[3]);                
    out_data -> acceleration.z = (int16_t) (accel_buf[4] << 8 | accel_buf[5]);

    const uint8_t GYRO_REG = 0x43;
    uint8_t gyro_buf[6];

    ESP_ERROR_CHECK(i2c_read_register(imu_cfg->bus, GYRO_REG, gyro_buf, imu_cfg->dev_address));

    out_data->gyro_rate_data.roll  = (int16_t)((gyro_buf[0] << 8) | gyro_buf[1]);
    out_data->gyro_rate_data.pitch = (int16_t)((gyro_buf[2] << 8) | gyro_buf[3]);
    out_data->gyro_rate_data.yaw   = (int16_t)((gyro_buf[4] << 8) | gyro_buf[5]);

    return ESP_OK;    
}