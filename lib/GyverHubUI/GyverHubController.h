#ifndef GYVER_HUB_CONTROLLER_H
#define GYVER_HUB_CONTROLLER_H

#include <Arduino.h>
#include "GyverHub.h"  

class GyverHubUI{
    private:
        int lamp_index = 0;

        const char* days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
        bool flags[7];
        bool auto_fan = 1;
        int bright = 0;
        int power = 0;
        int hue = 0;

        int plant_hour = 0;
        
        bool sliders[7] = {false};
     
       

        // Приватные методы для интерфейса
        void buildBrightnessSlider(gh::Builder& b);
        void buildPowerSlider(gh::Builder& b);
        void buildHueSlider(gh::Builder& b);
        void buildColorButtonsRow1(gh::Builder& b);
        void buildColorButtonsRow2(gh::Builder& b);
        void buildTimeControls(gh::Builder& b);
        void buildSwitches(gh::Builder& b);
        void buildSWhiteButtons(gh::Builder& b);
        void buildWatterController(gh::Builder& b);
        void buildFan(gh::Builder& b);

    public:
        void build(gh::Builder& b);
           int fan_power = 0;
           int plant_delay = 0;


};

#endif
