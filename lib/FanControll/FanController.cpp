#include "FanController.h"



FanController::FanController(uint8_t lm35Pin, uint8_t fanPin, uint32_t fanFreq, uint8_t fanChannel, uint8_t fanResolution, unsigned long readIntervalMs)
    :
      fanPin(fanPin),
      lm35Pin(lm35Pin),
      fanFreq(fanFreq),
      fanChannel(fanChannel),
      fanResolution(fanResolution),
      readIntervalMs(readIntervalMs),
      lastReadTime(0),
      temperature(NAN)
      
{}

void FanController::begin() {

    ledcSetup(fanChannel, fanFreq, fanResolution);
    ledcAttachPin(fanPin, fanChannel);
    
}

void FanController::update() {
      adcValue = analogRead(lm35Pin); // читаем ADC
        voltage = adcValue * 3.3 / 4095.0; // переводим в Вольты
        temperature = voltage * 100.0; // 10 мВ на °C → умножаем на 100
        speed = temperature*0.8;
if (auto_fan){
    setFanSpeed(speed, 1);

}
 
}

float FanController::getTemperature() const {
    return temperature;
}


void FanController::setFanSpeed(int speedy, bool change) {
    if(change)ledcWrite(fanChannel, speedy);
}
