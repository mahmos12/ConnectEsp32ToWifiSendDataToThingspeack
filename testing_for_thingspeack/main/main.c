#include <stdio.h>
#include "wifi.h"
#include "api.h"
#include "esp_http_client.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void){
    // ==================== STEG 1: INITIERA NVS ====================
    // // NVS = "Non-Volatile Storage" - minne som sparar data även utan ström
    esp_err_t ret = nvs_flash_init();

    // Om NVS är korrupt (fullt eller ny version), rensa och försök igen
    if(ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        printf("NVS problem, trying again\n");
        ESP_ERROR_CHECK(nvs_flash_erase());  // Rensa NVS
        ret = nvs_flash_init();              // Initiera igen
    }
    ESP_ERROR_CHECK(ret);  // Stanna om fortfarande fel
    printf("nvs init conneted\n");

    if(!wifi_init()){
        printf("Wifi could not be started\n");
        return;
    }

    Api_handle data = api_start();
    data->field1 = 60;
    data->field2 = 11;
    api_write(data);
    api_stop(data);

   vTaskDelay(pdMS_TO_TICKS(1000));
}