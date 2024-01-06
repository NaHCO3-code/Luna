/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <inttypes.h>
#include <string.h>
#include <algorithm>
#include "storage.h"
#include "server.h"
#include "luna.h"
extern "C" {
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_chip_info.h"

#include "wifi_sta.h"
}

static const char *TAG = "main";

extern "C" void app_main(void)
{
  ESP_LOGI(TAG, "hello");
  storage::init();
  server::wifi_start();
  server::http_start();
  luna::test();
  return;
}

