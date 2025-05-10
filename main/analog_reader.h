#ifndef ANALOG_READER_H
#define ANALOG_READER_H

#include "esp_err.h"
#include "driver/adc.h"

esp_err_t analog_reader_init(void);
esp_err_t analog_read(adc_channel_t channel, int* out_value);
esp_err_t analog_reader_deinit(void);

#endif // ANALOG_READER_H
