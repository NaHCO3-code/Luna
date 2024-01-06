idf_component_register(SRCS "server.cpp" "wifi_sta.cpp" "wifi_sta.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_http_server esp_wifi esp_event nvs_flash)
