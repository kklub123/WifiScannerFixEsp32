#include <Arduino.h>
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_event.h"

#define MAXIMUM_AP 20


static const char *auth_mode_type(wifi_auth_mode_t auth_mode)
{
  const char* types[] = {"OPEN", "WEP", "WPA PSK", "WPA2 PSK", "WPA WPA2 PSK", "MAX"};
  return types[auth_mode];
}

void wifiInit() { //First Called function
  ESP_ERROR_CHECK(nvs_flash_init());
  esp_netif_init();
  ESP_ERROR_CHECK(esp_event_loop_create_default());

  wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
  esp_netif_create_default_wifi_sta();
  ESP_ERROR_CHECK(esp_wifi_init(&wifi_config));
  ESP_ERROR_CHECK(esp_wifi_start());

}
///################################################################## <WIFI CONFIGS>
wifi_scan_config_t scan_config = {
  .ssid = 0,
  .bssid = 0,
  .channel = 0,
  .show_hidden = true,
  .scan_type = WIFI_SCAN_TYPE_PASSIVE,
};

wifi_country_t config = {
  .cc = "JP",
  .schan = 1,
  .nchan = 14,
  .policy = WIFI_COUNTRY_POLICY_AUTO,
};
///################################################################## </WIFI CONFIGS>

void setup() {
  Serial.printf("Setup start\n");
  wifiInit();
  Serial.printf("Setup done\n");


}
void loop()
{
  Serial.printf("\n");
  Serial.printf("\n");
  Serial.printf("\n");
  Serial.printf("Wifi scan start\n");
  ESP_ERROR_CHECK(esp_wifi_scan_start(&scan_config, true));
  Serial.printf("Wifi scan done\n");


  Serial.printf("AP record scan start\n");
  wifi_ap_record_t wifi_records[MAXIMUM_AP];

  uint16_t max_records = MAXIMUM_AP;
  
  ESP_ERROR_CHECK(esp_wifi_scan_get_ap_records(&max_records, wifi_records));
  Serial.printf("AP record scan done\n");

  
  Serial.printf("Number of Access Points Found: %d\n", max_records);
  Serial.printf("\n");
  Serial.printf("               SSID              | Channel | RSSI |   Authentication Mode \n");
  Serial.printf("***************************************************************\n");
  if (max_records==0) //may return Null instead of 0 check l8tr
  {
    Serial.printf("NO NETWORKS FOUND!\n");
  }else{ 
    for (int i = 0; i < max_records; i++){
      Serial.printf("%32s | %7d | %4d | %12s\n", (char *)wifi_records[i].ssid, wifi_records[i].primary, wifi_records[i].rssi, auth_mode_type(wifi_records[i].authmode),"\n");
    }
  }
  Serial.printf("***************************************************************\n");
}