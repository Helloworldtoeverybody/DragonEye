#include <Arduino.h>
#include <AutoWattering.h>
#include "NtpServer.h"
#include <string.h>
#include <esp_now.h>
#include <WiFi.h>
#define AP_SSID "11PM"
#define AP_PASS "251161251161"

esp_now_peer_info_t peerInfo;
Ntp ntp;

         const uint8_t  maxRetries     = 5;       // n — сколько раз пробуем отправить
  uint8_t        sendAttempts   = 0;       // сколько уже отправили
bool watering_confirmed = false;
int last_triggered_hour = -1;
uint8_t syncedChannel = 0;
// MAC-адрес приёмника (ESP32-C3)
uint8_t receiverMAC[] = {0x8C, 0xD0, 0xB2, 0xA8, 0x7A, 0x91};





typedef struct struct_response {
  bool end_flag;
    uint8_t wifi_channel;  // 👈 Добавляем это
} struct_response;

struct_response incomingResponse;

extern "C" {
  #include "esp_wifi.h"
}

void OnDataRecv(const uint8_t *mac, const uint8_t *data, int len) {
  struct_response response;
  memcpy(&response, data, sizeof(response));

  if (response.end_flag) {
    watering_confirmed = true;
    syncedChannel = response.wifi_channel;

    esp_wifi_set_channel(syncedChannel, WIFI_SECOND_CHAN_NONE);
    Serial.printf("🔄 synchronized to channel:  %d\n", syncedChannel);
    Serial.println("🌸 Watering confirmed by receiver!");
  }
  sendAttempts = maxRetries;
}
typedef struct struct_message {
  bool start_flag;
  int plant_delay;
} struct_message;

struct_message myData;

void Wattering::begin() {
// Set the device as a Station and Soft Access Point simultaneously
  WiFi.mode(WIFI_AP_STA);
  
  // Set device as a Wi-Fi Station
  WiFi.begin(AP_SSID, AP_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("🌌 Connecting to WiFi..");
  }
  Serial.print("🏡 Station IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.print("🏷️   Wi-Fi Channel: ");
  Serial.println(WiFi.channel());

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

esp_now_register_recv_cb(OnDataRecv);

memcpy(peerInfo.peer_addr, receiverMAC, 6);
peerInfo.channel = 0; // 👈 временно, будет обновлён
peerInfo.encrypt = false;

esp_now_add_peer(&peerInfo);
}

void Wattering::updateFlags(const char* day, bool value, int hour, int delay) {
  chosen_hour = hour;
  plant_delay = delay;

  Serial.print("⏰ Choosen hour is --> ");
  Serial.println(chosen_hour);
  Serial.print("⏱️  Delay is --> ");
  Serial.println(plant_delay);

  if (value && day != nullptr) {
    strncpy(planting_day, day, sizeof(planting_day) - 1);
    planting_day[sizeof(planting_day) - 1] = '\0';

    Serial.print("📌 Watering day set to --> ");
    Serial.println(planting_day);
  }
}

void Wattering::update() {
  const char* currentDay  = ntp.getDay();
  int         currentHour = ntp.getHours();

  // Сбрасываем флаг и счётчик при смене часа
  if (currentHour != last_triggered_hour) {
    watering_confirmed = false;
    sendAttempts       = 0;
  }

  // Если сейчас наш запланированный день и час...
  if (strcmp(planting_day, currentDay) == 0 &&
      chosen_hour == currentHour &&
      !watering_confirmed &&
      sendAttempts < maxRetries) {

    unsigned long now = millis();

    // Первая отправка сразу, последующие – по таймауту
    if (sendAttempts == 0 || now - lastSendTime >= retryInterval) {
      myData.start_flag  = true;
      myData.plant_delay = plant_delay;

      esp_err_t result = esp_now_send(receiverMAC,
                                      (uint8_t*)&myData,
                                      sizeof(myData));
      lastSendTime = now;
      sendAttempts++;

      if (result == ESP_OK) {
        Serial.printf("✅ Отправлено сообщение #%d\n", sendAttempts);
        // При первой отправке отмечаем, чтобы не слать заново до смены часа
        if (sendAttempts == 1) {
          last_triggered_hour = currentHour;
        }
      } else {
        Serial.printf("❌ Ошибка отправки #%d\n", sendAttempts);
      }

      // Если исчерпали попытки и нет ответа
      if (sendAttempts >= maxRetries && !watering_confirmed) {
        Serial.println("⚠️ Превышено количество попыток. Отправка остановлена.");
      }
    }
  }
}


void Wattering::plant() {
  Serial.println("💧 Planting triggered locally (optional).");
}




