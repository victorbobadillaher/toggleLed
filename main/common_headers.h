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
#include "esp_err.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lwip/ip_addr.h"   // For IPSTR/IP2STR

// Custom includes
#include "wifi_manager.h"
#include "websocket_server.h"
#include "imu_driver.h"

#include "app_config.h"