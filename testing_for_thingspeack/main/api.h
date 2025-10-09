#pragma once
#include "esp_http_client.h"
#include "wifi_api_info.h"
#include "stdlib.h"
#include "assert.h"

#define thingspeak_url "http://api.thingspeak.com/update"

typedef struct Api_data_t{
    int field1;
    int field2;
    esp_http_client_handle_t client;

} Api_data_t;

typedef Api_data_t *Api_handle;