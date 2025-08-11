#ifndef SMARTLIGHT_H
#define SMARTLIGHT_H



class Smartlight {
public:
    Smartlight();
    void turnOn();
    void turnOff();
    void setBrightness(int level);
    void setPower(bool power);
    void setEffect(int id);
    void update();
    void begin();
    void white();
    void setTemperature(int number);
    

    
private:
    int brightness;
    int currenteffect;
      bool fireReset = false;

    void fireRunOnce();

    int hue = 0; 


    void rainbow();
    void fire(bool reset);
    void effectStatic();
    void Random();
    void rainbowFlow();
    void LeavesEffect();
    void off();
    void Meteor();
    void Fireflies();
    


};

#endif