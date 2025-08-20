#ifndef AUTO_WATTERING_H
#define AUTO_WATTERING_H


#include <Arduino.h>

class Wattering{


    public:
        void updateFlags(const char* day, bool state, int hour, int plant_delay);
        void update();
        void plant();
        void begin();
          static const byte addresses[6][5]; 
        char planting_day[16] = "Nothing";
        bool wattering_flag;
         bool plant_flag;


    private:
        const char* days[7] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

        byte transmit_data[2];  // массив, хранящий передаваемые данные
        byte latest_data[2];    // массив, хранящий последние переданные данные
        int chosen_hour = 17;
        bool radio_flag;
        int plant_delay = 1;
        bool flag = 0;

        int last_hour;


       
        



};


#endif