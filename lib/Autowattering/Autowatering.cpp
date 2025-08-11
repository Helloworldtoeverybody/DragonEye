// === Autowatering.cpp ===
#include <Arduino.h> 
#include "nRF24L01.h"
#include "RF24.h"
#include <AutoWattering.h>
#include "NtpServer.h"
#include <string.h> 

#define CH_NUM 110   // номер канала (должен совпадать с приёмником)
//--------------------- НАСТРОЙКИ ----------------------

//--------------------- ДЛЯ РАЗРАБОТЧИКОВ -----------------------
// УРОВЕНЬ МОЩНОСТИ ПЕРЕДАТЧИКА
// На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
#define SIG_POWER RF24_PA_HIGH

// СКОРОСТЬ ОБМЕНА
// На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
// должна быть одинакова на приёмнике и передатчике!
// при самой низкой скорости имеем самую высокую чувствительность и дальность!!
// ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
#define SIG_SPEED RF24_1MBPS
//--------------------- ДЛЯ РАЗРАБОТЧИКОВ -----------------------

//--------------------- БИБЛИОТЕКИ ----------------------
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9, 53); // для Меги
//--------------------- БИБЛИОТЕКИ ----------------------

//--------------------- ПЕРЕМЕННЫЕ ----------------------
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; // возможные номера труб

int transmit_data[2];     // массив пересылаемых данных
int telemetry[2];         // массив принятых от приёмника данных телеметрии
byte rssi;
int trnsmtd_pack = 1, failed_pack;
unsigned long RSSI_timer;

Ntp ntp;
//--------------------- ПЕРЕМЕННЫЕ ----------------------



void radioSetup() {
  radio.begin();              // активировать модуль
  radio.setAutoAck(1);        // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    // (время между попыткой достучаться, число попыток)
  radio.enableAckPayload();   // разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);   // размер пакета, в байтах
  radio.openWritingPipe(address[0]);   // мы - труба 0, открываем канал для передачи данных
  radio.setChannel(CH_NUM);            // выбираем канал (в котором нет шумов!)
 radio.setPALevel (RF24_PA_HIGH);   // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp();         // начать работу
  radio.stopListening();   // не слушаем радиоэфир, мы передатчик
}










void Wattering::begin() {
  radioSetup();

}


void Wattering::updateFlags(const char* day, bool value, int hour, int delay) {
    chosen_hour = hour;
    plant_delay = delay;
    if (value && day != nullptr) {
        strncpy(planting_day, day, sizeof(planting_day) - 1);
        planting_day[sizeof(planting_day) - 1] = '\0';

        Serial.print("📌 Watering set to: ");
        Serial.println(planting_day);
    }
}


// Проверка: если сегодня день посадки, вызвать plant()
void Wattering::update() {  
    const char* currentDay = ntp.getDay();  // предполагается внешний объект ntp
    int currentHour = ntp.getHours();
    if (currentDay == nullptr) {
        Serial.println("⚠️ currentDay is null!");
        return;
    }


    if (planting_day[0] == '\0') {
        Serial.println("⚠️ planting_day is empty!");
        return;
    }

   

    if (strcmp(planting_day, currentDay) == 0 and chosen_hour == currentHour) {
        plant_flag = 1;
    }

    if(plant_flag){
        plant();
        //потом когда plant() вышлет все инфу и все подтвердиться тогда ставим plant_Flag в ноль
    }


      for (int i = 0; i < 2; i++) {               // в цикле от 0 до числа каналов
    if (transmit_data[i] != latest_data[i]) { // если есть изменения в transmit_data
      flag = 1;                               // поднять флаг отправки по радио
      latest_data[i] = transmit_data[i];      // запомнить последнее изменение
    }
  }

  if (flag == 1) {
    radio.powerUp();    // включить передатчик
    radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио
    flag = 0;           //опустить флаг
    radio.powerDown();  // выключить передатчик
  }
}



void Wattering::plant(){
  transmit_data[0] = 1;//пишем что пора начинать
  transmit_data[1] = plant_delay;




}




