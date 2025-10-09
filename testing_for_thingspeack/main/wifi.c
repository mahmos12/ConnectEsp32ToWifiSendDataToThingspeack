#include "wifi.h"
int wifi_init(){
    esp_err_t err;
    // ==================== STEG 2: INITIERA NETWORK INTERFACE ====================
    // Network interface = TCP/IP stack - "språket" för internetkommunikation
    printf("Starting network interface...\n");
    err = ESP_ERROR_CHECK(esp_netif_init());  // Starta TCP/IP systemet
    if(err == ESP_OK){ return 0; }
    printf("Network interface is up and ready\n");
    
    // ==================== STEG 3: SKAPA EVENT LOOP ==============
    // Event loop = System som lyssnar på händelser (t.ex. WiFi ansluten/frånkopplad)
    printf("Creating event loop...\n");
    err = ESP_ERROR_CHECK(esp_event_loop_create_default());  // Skapa händelsesystem
    if(err == ESP_OK){ return 0; }
    printf("Created event loop\n");
    
    // ==================== STEG 4: SKAPA WIFI STATION ====================
    // WiFi Station = När ESP32 ansluter till ett WiFi-nätverk (som en laptop)
    printf("Creating Wifi station...\n");
    err = esp_netif_t *wifi_sat = esp_netif_create_default_wifi_sta();  // Skapa WiFi-gränssnitt
    printf("Created Wifi station\n");
    
    // ==================== STEG 5: INITIERA WIFI-DRIVRUTINEN ====================
    printf("Initierar Wifi ...\n");
    wifi_init_config_t wif_config = WIFI_INIT_CONFIG_DEFAULT();  // Standard WiFi-inställningar
    err = ESP_ERROR_CHECK(esp_wifi_init(&wif_config));  // Starta WiFi-drivrutinen med inställningarna
    if(err == ESP_OK){ return 0; }
    printf("Initierarad Wifi!\n");
    
    // ==================== STEG 6: KONFIGURERA OCH STARTA WIFI ====================
    printf("5. Konfigurerar WiFi-läge...\n");
    err = ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));  // Sätt till Station-läge (ansluter till nätverk)
    if(err == ESP_OK){ return 0; }
    
    err = ESP_ERROR_CHECK(esp_wifi_start());  // Starta WiFi-radion på chippet
    if(err == ESP_OK){ return 0; }
    printf("WiFi startad i station-läge!\n");
    
    // ==================== STEG 7: KONFIGURERA WIFI-NÄTVERK ====================
    
    wifi_config_t wifi_config={
        .sta = {
            .ssid=WIFI_SSID,
            .password=WIFI_PASS,
        },
    };
    err = ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA,&wifi_config));
    if(err == ESP_OK){ return 0; }
    printf("WiFi konfigurerad för: %s\n", WIFI_SSID);

    // ==================== STEG 8: ANSLUT TILL WIFI ====================
    printf("7. Ansluter till WiFi...\n");
    err = ESP_ERROR_CHECK(esp_wifi_connect());
    if(err == ESP_OK){ return 0; }
    printf("Anslutning påbörjad!\n");
    printf("8. Väntar på WiFi-anslutning...\n");
        
    int wifi_timeout = 0;
    while(1) {
        wifi_ap_record_t ap_info;
        if(esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
            printf("WiFi ANSLUTEN! RSSI: %d dBm\n", ap_info.rssi);
            return 1;
        }
        
        printf(".");
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        
        if(wifi_timeout++ > 10) {  // Timeout efter 10 sekunder
            printf("WiFi timeout - ingen anslutning\n");
            return 0;
        }
    }
}

