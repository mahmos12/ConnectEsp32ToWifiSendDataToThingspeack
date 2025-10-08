#include <stdio.h>
#include "wifi_api_info.h" //
#include "esp_wifi.h"
#include "esp_http_client.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

const char* thingspeak_url = "http://api.thingspeak.com/update"; //oficial link

void app_main(void)
{
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

    // ==================== STEG 2: INITIERA NETWORK INTERFACE ====================
    // Network interface = TCP/IP stack - "språket" för internetkommunikation
    printf("Initierar network interface...\n");
    ESP_ERROR_CHECK(esp_netif_init());  // Starta TCP/IP systemet
    printf("network interface is redo\n");

    // ==================== STEG 3: SKAPA EVENT LOOP ==============
    // Event loop = System som lyssnar på händelser (t.ex. WiFi ansluten/frånkopplad)
    printf("Creating event loop...\n");
    ESP_ERROR_CHECK(esp_event_loop_create_default());  // Skapa händelsesystem
    printf("Created event loop\n");

    // ==================== STEG 4: SKAPA WIFI STATION ====================
    // WiFi Station = När ESP32 ansluter till ett WiFi-nätverk (som en laptop)
    printf("Creating Wifi Station...\n");
    esp_netif_t *wifi_sat = esp_netif_create_default_wifi_sta();  // Skapa WiFi-gränssnitt
    printf("Created wifi station\n");

    // ==================== STEG 5: INITIERA WIFI-DRIVRUTINEN ====================
    printf("Initierar Wifi ...\n");
    wifi_init_config_t wif_config = WIFI_INIT_CONFIG_DEFAULT();  // Standard WiFi-inställningar
    ESP_ERROR_CHECK(esp_wifi_init(&wif_config));  // Starta WiFi-drivrutinen med inställningarna
    printf("Initierarad wifi!\n");

    // ==================== STEG 6: KONFIGURERA OCH STARTA WIFI ====================
    printf("5. Konfigurerar WiFi-läge...\n");
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Sätt till Station-läge (ansluter till nätverk)

    ESP_ERROR_CHECK(esp_wifi_start());  // Starta WiFi-radion på chippet
    printf("WiFi startad i station-läge!\n");

    // ==================== STEG 7: KONFIGURERA WIFI-NÄTVERK ====================

    wifi_config_t wifi_config={
        .sta = {
            .ssid=WIFI_SSID,//din wifi namn 
            .password=WIFI_SSID,//wifi pass
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_config));
    printf("WiFi konfigurerad för: %s\n", WIFI_SSID);

    // ==================== STEG 8: ANSLUT TILL WIFI ====================
    printf("7. Ansluter till WiFi...\n");
    ESP_ERROR_CHECK(esp_wifi_connect());
    printf("Anslutning påbörjad!\n");
    printf("8. Väntar på WiFi-anslutning...\n");
        
    int wifi_timeout = 0;
    while(1) {
        wifi_ap_record_t ap_info;
        if(esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            printf("WiFi ANSLUTEN! RSSI: %d dBm\n", ap_info.rssi);
            break;
        }
        
        printf(".");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
        if(wifi_timeout++ > 10) {  // Timeout efter 10 sekunder
            printf("WiFi timeout - ingen anslutning\n");
            return;
        }
    }

    // ==================== STEG 9: TESTA THINGSPEAK ANSLUTNING ====================
    printf(" Testar ThingSpeak anslutning...\n");

    esp_http_client_config_t config={
        .url=thingspeak_url,
    };

    esp_http_client_handle_t client=esp_http_client_init(&config);
    
    char url_with_params[256];
    snprintf(url_with_params, sizeof(url_with_params),
                "%s?api_key=%s&field1=200&field2=600",
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
