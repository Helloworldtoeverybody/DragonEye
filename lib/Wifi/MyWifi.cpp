#include<Arduino.h>
#include<WiFi.h>
#include "MyWifi.h"


Wifi::Wifi(const char* ssidArg, const char* passwordArg) {
    AP_SSID =  ssidArg;
    AP_PASS = passwordArg;
}


void Wifi::begin(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS, 1);
while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\n✅ Wi-Fi connected");
  Serial.print("🏡 IP adress: ");
  Serial.println(WiFi.localIP());


}


bool Wifi::isConnected(){
  if(WiFi.status() == WL_CONNECTED) return 1;
  if(WiFi.status() == WL_CONNECT_FAILED) return 0;

}