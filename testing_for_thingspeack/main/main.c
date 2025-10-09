#include <stdio.h>
#include "wifi.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const char* thingspeak_url = "http://api.thingspeak.com/update"; //oficial link

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
        return 0;
    }

    // ==================== STEG 9: TESTA THINGSPEAK ANSLUTNING ====================
    printf(" Testar ThingSpeak anslutning...\n");

    esp_http_client_config_t config={
        .url=thingspeak_url,
    };

    esp_http_client_handle_t client=esp_http_client_init(&config);
    
    char url_with_params[256];
    snprintf(url_with_params, sizeof(url_with_params),
                "%s?api_key=%s&field1=10&field2=6",
                 thingspeak_url, API_WRITE_KEY);
    esp_http_client_set_url(client,url_with_params);

        // ==================== STEG 10: SKICKA DATA TILL THINGSPEAK ====================
    printf("9. Skickar data till ThingSpeak...\n");
    
    // Skicka HTTP request
    esp_err_t http_err = esp_http_client_perform(client);
    
    if (http_err == ESP_OK) {
        // Läs statuskod från ThingSpeak
        int status_code = esp_http_client_get_status_code(client);
        printf("ThingSpeak svarade med status: %d\n", status_code);
        
        if (status_code == 200) {
            printf("FRAMGÅNG! Data skickad till ThingSpeak!\n");
        } else {
            printf("ThingSpeak svarade med felkod: %d\n", status_code);
        }
    } else {
        printf("Kunde inte skicka till ThingSpeak: %s\n", esp_err_to_name(http_err));
    }
    
    // ==================== STEG 11: STÄDA UPP ====================
    esp_http_client_cleanup(client);
    printf("ThingSpeak test avslutat!\n");
}