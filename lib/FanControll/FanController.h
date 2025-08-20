#ifndef FAN_CONTROLLER_H
#define FAN_CONTROLLER_H

#include <Arduino.h>


class FanController {
public:
    FanController(uint8_t lm35Pin, uint8_t fanPin, uint32_t fanFreq = 20000, uint8_t fanChannel = 0, uint8_t fanResolution = 10, unsigned long readIntervalMs = 3000);
    void begin();
    void update();  // Вызывать в loop()
    float getTemperature() const;

    void setFanSpeed(int speed, bool change); // speed от 0 до (2^resolution -1)
    bool auto_fan = 0;

private:

    uint8_t fanPin;
    uint8_t lm35Pin;
    uint8_t fanChannel;
    uint8_t fanResolution;
    float speed = 0;
    uint32_t fanFreq;
    int adcValue;
     float voltage;
      float temperature;
    unsigned long readIntervalMs;

    unsigned long lastReadTime;
  

};

#endif
