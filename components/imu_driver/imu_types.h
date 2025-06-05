#pragma once

#include <stdint.h>
#include "driver/i2c_master.h"

typedef struct{
    i2c_master_bus_handle_t bus;
    uint8_t dev_address;
    uint16_t sample_rate_hz;
    uint8_t accel_range;
    uint8_t gyro_range;
    uint8_t filter_bandwidth;
}imu_driver_config_t;

typedef struct{
    int16_t x;
    int16_t y;
    int16_t z;
}acceleration_vector_t;

typedef struct{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
}gyro_rate_vector_t;

typedef struct{
    acceleration_vector_t acceleration;
    gyro_rate_vector_t gyro_rate_data;
} imu_driver_data_t;



