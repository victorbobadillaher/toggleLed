#include "imu_math.h"
#include "imu_types.h"

static float get_acceleration_angle(){
 // use tangent math to calculate the angle
    // angle = atan2(acceleration.y, acceleration.x) * (180.0 / M_PI);
    // return angle;
    // This is a placeholder, actual implementation will depend on the specific sensor and its data format
    return 0.0f; // Placeholder return value

}

static float get_gyro_rate_angle(){
    //use coriolis effect to calculate the angle
    
    return 0.0f;
}