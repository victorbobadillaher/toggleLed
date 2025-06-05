# pragma once

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "sdkconfig.h"

#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_event.h"
#include "esp_err.h"


#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/i2c_master.h"
#include "driver/i2c_slave.h"

#include "i2c_manager.h"

#include "imu_types.h"
#include "imu_math.h"
#include "imu_driver.h"
#include "imu_task.h"