////////////////SelfMade libraries/////////////////////////
#include<GyverHubController.h>
#include"MyWifi.h"
#include <Smartlight.h>
#include <Smartcurtain.h>
#include<NtpServer.h>


//////////////////Others//////////////////////
#include <Arduino.h>
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "esp32-hal-ledc.h"
#include "time.h"
#include <U8g2lib.h>
#include<GyverHub.h>
#include<Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "NtpServer.h"     // здесь важно, чтобы файл назывался именно так!
#include "Autowattering.h"





#define AP_SSID "11PM" //11PM//POCO C65 //NIKITOS//iPhone
#define AP_PASS "251161251161"//  "251161251161"//ig5dpur4bi7svpd//nikita1234//210587210587

const char* mqtt_server = "192.168.100.15";  // IP телефона с Mosquitto
const int mqtt_port = 1883;               // локальный порт Mosquitto

unsigned long lastUpdate_disp = 0;
const unsigned long interval_disp = 1000; // 1 секунда

int effect = 0;

byte brightness = 0;

//Fan settings
const int freq =20000;
const int ledChannel = 0;
const int resolution = 10;
const int ledPin = 26;//Fan pin

/////////////////////Objects/////////////////////////////////////
Smartlight lamp;
SmartCurtains curtain(14, 17, 26, 12, 25);//close_pin, open_pin, topLevel detector(hall sensor)
Wifi wifi(AP_SSID, AP_PASS);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C disp(U8G2_R0);
WiFiClient espClient;
PubSubClient client(espClient);
GyverHubUI ui;
GyverHub hub;


Ntp ntpTime;      // создаём объекты нужных классов
Wattering plant;



/////////////////Recieving topics from Alexa(WQTT.ru)/////////////////////////////
void callback(char* topic, byte* payload, unsigned int length) {

  char msgBuffer[length + 1];
  memcpy(msgBuffer, payload, length);
  msgBuffer[length] = '\0';
  String msg = String(msgBuffer);




  if (String(topic) == "Brightness") {
    byte bright = msg.toInt();
    brightness = bright;
    lamp.setBrightness(bright);
    Serial.printf("Brightness -> %d\n", brightness);
    
  }
  if (String(topic) == "power") {
    bool state = msg.toInt();
      lamp.setPower(state);
    Serial.printf("Power -> %d\n", state);
    
  }
//curtain topics




  


  if (String(topic) == "curtainsMode") {
    int curtain_power = msg.toInt();

switch (curtain_power)
{
case 1 :

curtain.open();
Serial.println("Curtain openning");
  
  break;
case 2:
  curtain.close();
  Serial.println("Curtain closing");
  break;
case 3:
  curtain.stop();
    Serial.println("Curtain stop!");
  break;
default:
  curtain.stop();//By default let it chill)))
  break;
}
   



    Serial.printf("Curtain_power -> %d\n", curtain_power);
  }


//lamp_topics
if (strcmp(topic, "color") == 0) {
  int r, g, b;
  sscanf(msg.c_str(), "%d,%d,%d", &r, &g, &b);
  Serial.printf("Color R:%d G:%d B:%d\n", r, g, b);

  int red = r;
  int green = g;
  int blue = b;

  
}



  if (String(topic) == "mode") {
    int modeValue = msg.toInt();
    lamp.setEffect(modeValue);
    Serial.printf("Effect -> %d\n", modeValue);
  }
}





/////////////////////////////Setup////////////////////////
void setup(){
Serial.begin(115200);

ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(ledPin, ledChannel);
  wifi.begin();


  while (wifi.isConnected() != 1) {
    delay(500);
    Serial.print(".");
  }

  
lamp.begin();
disp.begin();
plant.begin();
hub.begin();

hub.onBuild([](gh::Builder& b){
    ui.build(b);
});






  lamp.setPower(true); 
  lamp.setEffect(2);   


  ntpTime.begin();
  hub.mqtt.config("broker.emqx.io", 1883);

 
   hub.config(F("MyDevices"), F("🐲🔥🌈ESP"), F(""));
 


  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  



  

Serial.println("💣🧨🎊Enjoy👍☘️🐉");

curtain.stop();
   

 
 curtain.begin();




}



void reconnect() {
  while (!client.connected()) {
    Serial.print("🌎Connectinng to MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println(" 🎉Sueccess!🧨");
      
      client.subscribe("#");

    } else {
      Serial.print(" Error: ");
      Serial.print(client.state());
      delay(3000);
    }
  }
}







void loop() {
  int now_hour = ntpTime.getHours();
  int now_minute = ntpTime.getMinutes();
  int now_second = ntpTime.getSeconds();




 unsigned long now = millis();
  if (now - lastUpdate_disp >= interval_disp) {
    lastUpdate_disp = now;
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", now_hour, now_minute);
    disp.clearBuffer();
    disp.setFont(u8g2_font_helvB24_tf);//u8g2_font_helvB24_te
    disp.setCursor(0, 50);
    disp.print(timeStr);
    disp.sendBuffer();
  }
/*
    char timeStr[6];
    sprintf(timeStr, "%02d:%02d", now_hour, now_minute);

    disp.clearBuffer();
    disp.setFont(u8g2_font_helvB24_tf);
    disp.setCursor(0, 40);
    disp.print("Hawai");
    disp.sendBuffer();
  */


  int FanPower = ui.fan_power;
    ledcWrite(ledChannel, FanPower);

hub.tick();

  lamp.update();
  curtain.update();
plant.update();
  



  if (!client.connected()) {
    reconnect();
  }
  client.loop();




}









/*
Йоу!:)

1. нужно сделать плавное включение и выключение светильников.
2. Добавить новых крутых эффектов
3. Сделать все в классы ии добавитть файлы юр и cpp. 
4. Закомитить все на github.
5. ??????????????????????.



↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑
Этот есть у чата GPT.


Для крутого фокуса включай клафишу F11 и увидишь :)



















*/


