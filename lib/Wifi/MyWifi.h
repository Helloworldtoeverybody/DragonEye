#ifndef MyWifi
#define MyWifi

class Wifi{
    public:
        void begin();
        bool isConnected();

        Wifi(const char* ssidArg, const char* passwordArg);



    private:
        const char*  AP_SSID;
        const char*  AP_PASS;

        //#define AP_SSID "11PM" //11PM//POCO C65 //NIKITOS
        //#define AP_PASS "251161251161"//  "251161251161"//ig5dpur4bi7svpd//nikita1234

        

};



#endif