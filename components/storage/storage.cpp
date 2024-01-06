#include <stdio.h>
#include "storage.h"
extern "C" {
#include "esp_log.h"
#include "esp_spiffs.h"
}

static const char *TAG = "storage";

namespace storage{
  esp_err_t init(void);
}

esp_err_t storage::init(void){
  ESP_LOGI(TAG, "Initializing SPIFFS");

  esp_err_t ret;
  esp_vfs_spiffs_conf_t conf = {
    .base_path = "/spiffs",
    .partition_label = NULL,
    .max_files = 5,
    .format_if_mount_failed = true,
  };
  
  ret = esp_vfs_spiffs_register(&conf);
  if (ret != ESP_OK) {
    if (ret == ESP_FAIL) {
      ESP_LOGE(TAG, "Failed to mount or format filesystem");
    } else if (ret == ESP_ERR_NOT_FOUND) {
      ESP_LOGE(TAG, "Failed to find SPIFFS partition");
    } else {
      ESP_LOGE(TAG, "Failed to initialize SPIFFS (%s)", esp_err_to_name(ret));
    }
    return ESP_FAIL;
  }

  ESP_LOGI(TAG, "Performing SPIFFS_check().");
  ret = esp_spiffs_check(conf.partition_label);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
    return ESP_FAIL;
  } else {
    ESP_LOGI(TAG, "SPIFFS_check() successful");
  }

  size_t total = 0, used = 0;
  ret = esp_spiffs_info(conf.partition_label, &total, &used);
  if (ret != ESP_OK) {
    ESP_LOGE(TAG, "Failed to get SPIFFS partition information (%s). Formatting...", esp_err_to_name(ret));
    esp_spiffs_format(conf.partition_label);
    return ESP_FAIL;
  } else {
    ESP_LOGI(TAG, "Partition size: total: %d, used: %d", total, used);
  }

  // Check consistency of reported partiton size info.
  if (used > total) {
    ESP_LOGW(TAG, "Number of used bytes cannot be larger than total. Performing SPIFFS_check().");
    ret = esp_spiffs_check(conf.partition_label);
    if (ret != ESP_OK) {
      ESP_LOGE(TAG, "SPIFFS_check() failed (%s)", esp_err_to_name(ret));
      return ESP_FAIL;
    } else {
      ESP_LOGI(TAG, "SPIFFS_check() successful");
    }
  }

  return ESP_OK;
}