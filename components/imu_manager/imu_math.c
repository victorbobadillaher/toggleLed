#include "imu_math.h"
#include "imu_types.h"
#include <math.h>

static float get_acceleration_angle(acceleration_vector_t acceleration){
 // use tangent math to calculate the angle
    float angle = atan2(acceleration.y, acceleration.x) * (180.0 / M_PI);
    return angle;
}

static float get_gyro_rate_angle(gyro_rate_vector_t gyro_rate){
    //use coriolis effect to calculate the angle
    
    return 0.0f;
}

static imu_data_t imu_mapping_task(imu_data_t *raw_data)
{
    imu_data_t processed_data;

    // Example processing: just copy raw data for now
    processed_data.acceleration.x = raw_data->acceleration.x;
    processed_data.acceleration.y = raw_data->acceleration.y;
    processed_data.acceleration.z = raw_data->acceleration.z;

    processed_data.gyro_rate_data.roll = raw_data->gyro_rate_data.roll;
    processed_data.gyro_rate_data.pitch = raw_data->gyro_rate_data.pitch;
    processed_data.gyro_rate_data.yaw = raw_data->gyro_rate_data.yaw;

    return processed_data;
}

/*
static void imu_complementary_filter()
{


}

static void imu_mahony_filter()
{
    // Implement Mahony filter logic here
    // This is a placeholder, actual implementation will depend on the specific sensor and its data format
}*/

