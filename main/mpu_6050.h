#ifndef MPU6050_H
#define MPU6050_H

#include <stdint.h>

typedef enum {
    ACCEL_2G = 0,
    ACCEL_4G = 1,
    ACCEL_8G = 2,
    ACCEL_16G = 3
} accel_range_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

void mpu6050_set_accel_range(accel_range_t range);
float convert_raw_to_g(int16_t raw, accel_range_t range);
vec3_t convert_all_to_g(int16_t raw_x, int16_t raw_y, int16_t raw_z, accel_range_t range);

#endif

