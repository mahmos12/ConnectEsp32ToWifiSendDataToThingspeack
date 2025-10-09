int api_start(){
    // ==================== STEG 9: TESTA THINGSPEAK ANSLUTNING ====================
    printf(" Testar ThingSpeak anslutning...\n");
    esp_http_client_config_t config = {
        .url=thingspeak_url,
    };
    esp_http_client_handle_t client = esp_http_client_init(&config);
}


int write_api(){
    // ==================== STEG 10: SKICKA DATA TILL THINGSPEAK ====================
    char url_with_params[256];
    snprintf(url_with_params, sizeof(url_with_params),
                "%s?api_key=%s&field1=10&field2=6",
                 thingspeak_url, API_WRITE_KEY);
    esp_http_client_set_url(client,url_with_params);
    
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

void read_from_thingspeack(){
    
}