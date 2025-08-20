#include <Arduino.h>
#include <AutoWattering.h>
#include "NtpServer.h"
#include <string.h>
#include <esp_now.h>
#include <WiFi.h>

esp_now_peer_info_t peerInfo;
Ntp ntp;

bool watering_confirmed = false;
int last_triggered_hour = -1;

// MAC-адрес приёмника (ESP32-C3)
uint8_t receiverMAC[] = {0x8C, 0xD0, 0xB2, 0xA8, 0x7A, 0x91};

typedef struct struct_response {
  bool end_flag;
} struct_response;

struct_response incomingResponse;

void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  struct_response response;
  memcpy(&response, data, sizeof(response));

  if (response.end_flag) {
    watering_confirmed = true; // ✅ This is the missing piece
    Serial.println("🌸 Watering confirmed by receiver!");
  }
}

typedef struct struct_message {
  bool start_flag;
  int plant_delay;
} struct_message;

struct_message myData;

void Wattering::begin() {
  WiFi.mode(WIFI_STA);

   WiFi.channel(6); // Make sure both devices use the same channel


  if (esp_now_init() != ESP_OK) {
    Serial.println("❌ ESP-NOW init failed");
    return;
  }

esp_now_register_recv_cb(OnDataRecv);

 memcpy(peerInfo.peer_addr, receiverMAC, 6); // 👈 Direct peer communication
  peerInfo.channel = 6;     // 👈 Match this to WiFi.channel()
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("❌ Failed to add peer");
    return;
  }

  Serial.println("✅ ESP-NOW ready");
  Serial.print("📡 This device's MAC address: ");
  Serial.println(WiFi.macAddress());
  Serial.print("📶 Current WiFi channel: ");
Serial.println(WiFi.channel());
}

void Wattering::updateFlags(const char* day, bool value, int hour, int delay) {
  chosen_hour = hour;
  plant_delay = delay;

  Serial.print("⏰Choosen hour is --> ");
  Serial.println(chosen_hour);
  Serial.print("⏱️Delay is -->");
  Serial.println(plant_delay);

  if (value && day != nullptr) {
    strncpy(planting_day, day, sizeof(planting_day) - 1);
    planting_day[sizeof(planting_day) - 1] = '\0';

    Serial.print("📌 Watering set to -->");
    Serial.println(planting_day);
  }
}

void Wattering::update() {
  const char* currentDay = ntp.getDay();
  int currentHour = ntp.getHours();

  if (strcmp(planting_day, currentDay) == 0 &&
      chosen_hour == currentHour &&
      !watering_confirmed &&
      last_triggered_hour != currentHour) {

    myData.start_flag = true;
    myData.plant_delay = plant_delay;

    esp_err_t result = esp_now_send(receiverMAC, (uint8_t*)&myData, sizeof(myData));


    if (result == ESP_OK) {
      Serial.println("✅ Данные отправлены");
      last_triggered_hour = currentHour;
    } else {
      Serial.println("❌ Ошибка отправки");
    }
  }

  if (currentHour != last_triggered_hour) {
    watering_confirmed = false;
  }
}

void Wattering::plant() {
  Serial.println("💧 Planting triggered locally (optional).");
}

