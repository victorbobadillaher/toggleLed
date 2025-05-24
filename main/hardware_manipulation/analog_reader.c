#include "analog_reader.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "analog_reader";

static adc_oneshot_unit_handle_t adc1_handle = NULL;

esp_err_t analog_reader_init(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT_1,
    };

    return adc_oneshot_new_unit(&init_config, &adc1_handle);
}

esp_err_t analog_read(adc_channel_t channel, int* out_value)
{
    if (!adc1_handle) return ESP_ERR_INVALID_STATE;

    adc_oneshot_chan_cfg_t chan_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_11,  // Adjust for 0-3.3V range
    };

    esp_err_t err = adc_oneshot_config_channel(adc1_handle, channel, &chan_config);
    if (err != ESP_OK) return err;

    int raw = 0;
    err = adc_oneshot_read(adc1_handle, channel, &raw);
    if (err == ESP_OK) {
        *out_value = raw;
        ESP_LOGI(TAG, "ADC%d_CH%d raw: %d", ADC_UNIT_1, channel, raw);
    }

    return err;
}

esp_err_t analog_reader_deinit(void)
{
    return adc_oneshot_del_unit(adc1_handle);
}
