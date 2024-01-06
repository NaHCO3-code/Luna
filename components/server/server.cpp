#include <stdio.h>
#include "server.h"
#include "dirent.h"

extern "C"{
#include "esp_log.h"
#include "esp_http_server.h"
#include "sys/unistd.h"
}

static const char *TAG = "server";

esp_err_t send_file(httpd_req_t *req, const char* path){
  // 打开文件
  FILE* f = fopen(path, "r");
  if(f == NULL){
    ESP_LOGE(TAG, "Failed to open file");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  // 获取文件大小
  fseek(f, 0, SEEK_END);
  size_t fsize = ftell(f);
  fseek(f, 0, SEEK_SET);

  // 建立缓冲区
  const size_t block_size = 1024*100;
  char* buf = (char*)malloc(block_size+1);
  if(buf == NULL){
    ESP_LOGE(TAG, "Failed to malloc");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }

  // 发送响应
  while(fsize > block_size){
    fread(buf, sizeof(char), block_size, f);
    fsize -= block_size;
    buf[block_size] = '\0';
    if(httpd_resp_send_chunk(req, buf, block_size) != ESP_OK){
      ESP_LOGE(TAG, "Failed to send chunk");
      return ESP_FAIL;
    }
  }
  fread(buf, sizeof(char), fsize, f);
  buf[fsize] = '\0';
  if(httpd_resp_send_chunk(req, buf, fsize) != ESP_OK){
    ESP_LOGE(TAG, "Failed to send chunk");
    return ESP_FAIL;
  }

  // 结束响应
  httpd_resp_send_chunk(req, NULL, 0);

  // 释放缓冲区
  free(buf);
  fclose(f);
  return ESP_OK;
}

esp_err_t get_index(httpd_req_t *req){
  send_file(req, "/spiffs/index.html");

  return ESP_OK;
}

esp_err_t get_file_list(httpd_req_t *req){
  struct dirent *d;
  DIR *dir = opendir("/spiffs/");
  if(dir == NULL){
    ESP_LOGE(TAG, "Failed to open dir");
    httpd_resp_send_500(req);
    return ESP_FAIL;
  }
  while((d = readdir(dir)) != NULL){
    if(d->d_name[0] == '.'){
      continue;
    }
    httpd_resp_sendstr_chunk(req, d->d_name);
    httpd_resp_sendstr_chunk(req, "\n");
  }
  closedir(dir);
  httpd_resp_send_chunk(req, NULL, 0);
  return ESP_OK;
}

/* GET /uri 的 URI 处理结构 */
httpd_uri_t uri_get_index = {
  .uri      = "/",
  .method   = HTTP_GET,
  .handler  = get_index,
  .user_ctx = NULL
};

httpd_uri_t uri_get_file_list = {
 .uri      = "/list",
 .method   = HTTP_GET,
 .handler  = get_file_list,
 .user_ctx = NULL
};

void server::http_start(){
  httpd_config_t conf = HTTPD_DEFAULT_CONFIG();
  httpd_handle_t server = NULL;
  if(httpd_start(&server, &conf) == ESP_OK){
    httpd_register_uri_handler(server, &uri_get_index);
    httpd_register_uri_handler(server, &uri_get_file_list);
  }
}