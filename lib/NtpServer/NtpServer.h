#ifndef NtpServer
#define NtpServer


#include <Arduino.h>

class Ntp{
    public:

        void begin();
    
        void printLocalTime();
        int getHours();
        int getMinutes();
        int getSeconds();
        const char* getDay();
        



    private:
        const char* days[7] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        const char* _ntpServer =   "pool.ntp.org";
        const int _daylightOffset_sec = 3 * 3600;
        const long _gmtOffset_sec = 0;
        
};



#endif