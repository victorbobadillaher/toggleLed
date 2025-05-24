



void mpu6050_read_task(void *params)
{
    struct mpu_task_params {
        i2c_master_bus_handle_t bus;
        uint8_t dev_addr;
        uint16_t sample_rate_hz;
    };

    struct mpu_task_params *cfg = (struct mpu_task_params *)params;

    const uint8_t ACCEL_START_REG = 0x3B;
    uint8_t raw_data[6];
    int16_t accel_x, accel_y, accel_z;

    TickType_t delay_ticks = pdMS_TO_TICKS(1000 / cfg->sample_rate_hz);

    while (1)
    {
        i2c_master_dev_handle_t dev_handle;

        i2c_device_config_t dev_cfg = {
            .dev_addr_length = I2C_ADDR_BIT_LEN_7,
            .device_address = cfg->dev_addr,
            .scl_speed_hz = 100000,
        };

        ESP_ERROR_CHECK(i2c_master_bus_add_device(cfg->bus, &dev_cfg, &dev_handle));

        // Tell the sensor we want to read from 0x3B
        ESP_ERROR_CHECK(i2c_master_transmit(dev_handle, &ACCEL_START_REG, 1, -1));

        // Read 6 bytes: XH, XL, YH, YL, ZH, ZL
        ESP_ERROR_CHECK(i2c_master_receive(dev_handle, raw_data, 6, -1));

        // Parse 16-bit signed values
        accel_x = (raw_data[0] << 8) | raw_data[1];
        accel_y = (raw_data[2] << 8) | raw_data[3];
        accel_z = (raw_data[4] << 8) | raw_data[5];

        printf("Accel X: %d\tY: %d\tZ: %d\n", accel_x, accel_y, accel_z);

        ESP_ERROR_CHECK(i2c_master_bus_rm_device(dev_handle));

        vTaskDelay(delay_ticks);
    }
}