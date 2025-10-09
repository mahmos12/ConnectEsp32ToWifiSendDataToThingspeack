idf_component_register(SRCS "read_from_thingspeack.c" "wifi.c" "main.c"
                    INCLUDE_DIRS "."
                    REQUIRES nvs_flash esp_wifi esp_http_client esp_timer)