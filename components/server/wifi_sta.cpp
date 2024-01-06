#include <stdio.h>
#include "server.h"

extern "C"{
#include "wifi_sta.h"
#include "esp_log.h"
#include "nvs_flash.h"
}

static const char *TAG = "wifi station";

void server::wifi_start(void)
{
  //Initialize NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
  wifi_init_sta();
}
