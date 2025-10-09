#include "api.h"

Api_handle api_start(){
    Api_handle data = (Api_handle)malloc(sizeof(Api_data_t));
    assert(data != NULL);
    // ==================== STEG 9: TESTA THINGSPEAK ANSLUTNING ====================
    printf(" Testar ThingSpeak anslutning...\n");
    esp_http_client_config_t config = { .url = THINGSPEAK_URL };
    data->client = esp_http_client_init(&config);
    return data;
}

int api_write(Api_handle data){
    // ==================== STEG 10: SKICKA DATA TILL THINGSPEAK ====================
    char url_with_params[256];
    snprintf(url_with_params, sizeof(url_with_params),
                "%s/update?api_key=%s&field1=%d&field2=%d",
                 THINGSPEAK_URL, API_WRITE_KEY,
                 data->field1, data->field2);
    esp_http_client_set_url(data->client, url_with_params);
    printf("9. Skickar data till ThingSpeak...\n");
    // Skicka HTTP request
    esp_err_t http_err = esp_http_client_perform(data->client);
    if (http_err == ESP_OK) {
        // Läs statuskod från ThingSpeak
        int status_code = esp_http_client_get_status_code(data->client);
        printf("ThingSpeak svarade med status: %d\n", status_code);
        if (status_code == 200) {
            printf("FRAMGÅNG! Data skickad till ThingSpeak!\n");
            return 1;
        } else {
            printf("ThingSpeak svarade med felkod: %d\n", status_code);
            return -1;
        }
    } else {
        printf("Kunde inte skicka till ThingSpeak: %s\n", esp_err_to_name(http_err));
        return 0;
    }
}

void api_stop(Api_handle data){
    // ==================== STEG 11: STÄDA UPP ====================
    esp_http_client_cleanup(data->client);
    printf("ThingSpeak test avslutat!\n");
}

void api_read(Api_handle data){
    int channel_ID = 3103134;
    char url_with_params[256];
    snprintf(url_with_params, sizeof(url_with_params),
                "%s?api_key=%s&field1=%d&field2=%d",
                 READ_THINGSPEAK_URL, API_READ_KEY,
                 data->field1, data->field2);
    esp_http_client_set_url(data->client, url_with_params);
    printf("9. Skickar data till ThingSpeak...\n");
    // Skicka HTTP request
    esp_err_t http_err = esp_http_client_perform(data->client);

}