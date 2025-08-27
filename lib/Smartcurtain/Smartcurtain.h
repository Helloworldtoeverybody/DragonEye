#ifndef SMARTCURTAIN_H
#define SMARTCURTAIN_H



class SmartCurtains{
public:
    SmartCurtains(int openPin, int closePin, int b_openPin, int b_closePin, int top);
    void begin();
    void open();
    void close();
    void stop();
    void setPosition(int pos);
    void update();
    

    unsigned long time_out;
    

private:

    int openPin;
    int closePin;
    int b_openPin;
    int b_closePin;
    int top;
    int targetPosition;
    int currentPosition;
    bool isMoving;
    unsigned long lastUpdate;
    unsigned long timer;

    int top_point = 50;

};










#endif