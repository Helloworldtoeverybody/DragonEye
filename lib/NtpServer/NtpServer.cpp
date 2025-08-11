// === NtpServer.cpp (добавь этот файл, если его нет) ===
#include "NtpServer.h"
#include <time.h>
#include <stdio.h>
#include<Arduino.h>

void Ntp::begin() {
    configTime(_gmtOffset_sec, _daylightOffset_sec, _ntpServer);
}
const char* Ntp::getDay(){
    struct tm timeinfo;
if (!getLocalTime(&timeinfo)) {
    Serial.println("❌ Не удалось получить время.");
   
  }
    return days[timeinfo.tm_wday];
}
int Ntp::getHours(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("❌ Не удалось получить время.");
        return 0;  // Или любое значение по умолчанию при ошибке
    }
    return timeinfo.tm_hour;
}

int Ntp::getMinutes(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("❌ Не удалось получить время.");
        return 0;
    }
    return timeinfo.tm_min;
}

int Ntp::getSeconds(){
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("❌ Не удалось получить время.");
        return 0;
    }
    return timeinfo.tm_sec;
}