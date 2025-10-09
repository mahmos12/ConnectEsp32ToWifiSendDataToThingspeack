idf_component_register(SRCS "api.c" "wifi.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash esp_wifi esp_http_client esp_timer)