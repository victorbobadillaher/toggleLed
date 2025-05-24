#include "sensor_helper.h"

typedef struct sensor_t {
    int id;
    char name[50];
    char* type;  // "i2c", "adc", etc.
    float value;
    float sampling_rate;  // in Hz
    int address;          // for I2C

    // Function pointers for polymorphism
    esp_err_t (*init)(struct sensor_t* self);
    esp_err_t (*read)(struct sensor_t* self);
    esp_err_t (*deinit)(struct sensor_t* self);

    // Optional RTOS task handle for continuous reading
    TaskHandle_t task_handle;

} sensor_t;
