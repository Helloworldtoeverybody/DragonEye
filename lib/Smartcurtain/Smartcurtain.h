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
    int top_point = 0;
    bool change_flag = 0;
    

private:
bool top_flag = 0;
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

};










#endif