idf_component_register(SRCS "wifi.cpp" "wifi_init_sta.c"
                    INCLUDE_DIRS "include"
                    REQUIRES esp_wifi esp_event nvs_flash)
