#pragma once

#include "imu_common.h"


esp_err_t imu_driver_init(imu_config_t imu_cfg);
esp_err_t imu_driver_read(imu_data_t * out_data, imu_config_t * imu_cfg);



