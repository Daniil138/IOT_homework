#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h>

String id() {
    uint8_t mac[WL_MAC_ADDR_LENGTH];
    WiFi.softAPmacAddress(mac);
    String macID = String(mac[WL_MAC_ADDR_LENGTH-2], HEX); 
    macID = macID + String(mac[WL_MAC_ADDR_LENGTH-1], HEX);   
    return macID;
}

void start_AP_mode() {
    
    IPAddress AP_IP(192,168,4,1);
    Serial.print("Attempt to start WiFi AP");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(AP_IP,AP_IP,IPAddress(255,255,255,0));
    WiFi.softAP(SSID_AP + "_" + id(), PASSWORD_AP);
    Serial.print("WiFI AP is up, see " + SSID_AP);
}

void start_client_mode() {
    
}

void init_WIFI(bool mode) {
    if (mode == WIFI_START_MODE_CLIENT) {
        start_client_mode();
    } else {
        start_AP_mode();
    }
}
