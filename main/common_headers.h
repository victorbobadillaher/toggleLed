#pragma once
// Standard C library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// FreeRTOS includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"

// ESP-IDF common includes
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#include "esp_system.h"
#include "esp_http_server.h"
#include "lwip/ip_addr.h"   // For IPSTR/IP2STR
#include "sdkconfig.h"

// Custom includes
#include "wifi_manager.h"
#include "websocket_manager.h"

//commnications includes
#include "driver/i2c_master.h"

