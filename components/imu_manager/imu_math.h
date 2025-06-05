#pragma once

#include <math.h>
#include <stdint.h>
#include "imu_common.h"

static float get_acceleration_angle(acceleration_vector_t acceleration);

static float get_gyro_rate_angle(gyro_rate_vector_t gyro_rate);

static imu_data_t imu_mapping_task(imu_data_t *raw_data);


/*
static void imu_complementary_filter()
{


}

static void imu_mahony_filter()
{
    // Implement Mahony filter logic here
    // This is a placeholder, actual implementation will depend on the specific sensor and its data format
}*/

