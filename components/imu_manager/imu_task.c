#include "imu_task.h"
#include "websocket_manager.h"

static const char *TAG = "IMU_TASK";

void imu_task(void *params)
{
    imu_config_t *imu_cfg =  (imu_config_t*) params;

    if(imu_driver_init(*imu_cfg) != ESP_OK)
    {
        ESP_LOGE(TAG, "Failed to initialize IMU driver");
        vTaskDelete(NULL);
        return;
    }

    ESP_LOGI(TAG, "IMU driver initialized successfully");

    uint16_t sample_rate = imu_cfg->sample_rate_hz;

    TickType_t delay_ticks = pdMS_TO_TICKS(1000 /sample_rate);

    imu_data_t raw_data;

    while (1)
    {
       if (imu_driver_read(&raw_data, imu_cfg) == ESP_OK)
        {
            ESP_LOGI(TAG, "Accel X:%d Y:%d Z:%d",
                     raw_data.acceleration.x,
                     raw_data.acceleration.y,
                     raw_data.acceleration.z);

            // 🔥 Send to WebSocket if client connected

            ESP_LOGI(TAG,"web socket server is: %i", websocket_server_is_connected());

            if (websocket_server_is_connected()) {
                char payload[128];
                snprintf(payload, sizeof(payload),
                         "{\"ax\":%d,\"ay\":%d,\"az\":%d}",
                         raw_data.acceleration.x,
                         raw_data.acceleration.y,
                         raw_data.acceleration.z);

                websocket_manager_send_text(payload);  // BOOM
            }
        }
        else
        {
            ESP_LOGW(TAG, "Failed to read from IMU");
        }

        vTaskDelay(delay_ticks);
    }
}
