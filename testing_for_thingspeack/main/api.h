#pragma once
#include "esp_http_client.h"
#include "wifi_api_info.h"
#include "stdlib.h"
#include "assert.h"

#define WRITE_THINGSPEAK_URL "http://api.thingspeak.com"
#define READ_THINGSPEAK_URL "http://api.thingspeak.com/channels"

typedef struct Api_data_t{
    int field1;
    int field2;
    esp_http_client_handle_t client;

} Api_data_t;

typedef Api_data_t *Api_handle;

Api_handle api_start();
int api_write(Api_handle data);
void api_stop(Api_handle data);