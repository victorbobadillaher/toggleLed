#include "mpu6050.h"

static const float accel_scales[] = {
    16384.0, 8192.0, 4096.0, 2048.0
};

static accel_range_t current_range = ACCEL_2G;  // Default

void mpu6050_set_accel_range(accel_range_t range) {
    current_range = range;
    // Optionally: Write to ACCEL_CONFIG register (0x1C)
    // MPU6050_writeRegister(0x1C, range << 3);
}

float convert_raw_to_g(int16_t raw, accel_range_t range) {
    return (float)raw / accel_scales[range];
}

vec3_t convert_all_to_g(int16_t raw_x, int16_t raw_y, int16_t raw_z, accel_range_t range) {
    vec3_t accel_g;
    accel_g.x = convert_raw_to_g(raw_x, range);
    accel_g.y = convert_raw_to_g(raw_y, range);
    accel_g.z = convert_raw_to_g(raw_z, range);
    return accel_g;
}
