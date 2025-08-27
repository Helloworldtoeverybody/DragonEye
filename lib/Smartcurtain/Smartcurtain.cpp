#include <Smartcurtain.h>
#include<Arduino.h>


    bool circle_flag = 0;
int counter;

SmartCurtains::SmartCurtains(int openPin, int closePin, int b_openPin, int b_closePin, int top)
: openPin(openPin), closePin(closePin), b_openPin(b_openPin),  b_closePin(b_closePin), top(top), targetPosition(0), currentPosition(0), isMoving(false), lastUpdate(0)
{}



void SmartCurtains::begin(){


    pinMode(openPin, OUTPUT);
    pinMode(closePin, OUTPUT);
    
    pinMode(b_openPin, OUTPUT);
    pinMode(b_closePin, OUTPUT);

    pinMode(top, INPUT);

    stop();

}


void SmartCurtains::open() {


    digitalWrite(openPin, 200);
    digitalWrite(closePin, LOW);


}

void SmartCurtains::close() {

    digitalWrite(openPin, LOW);
    digitalWrite(closePin, 200);


 

}



void SmartCurtains::stop() {
    digitalWrite(openPin, LOW);
    digitalWrite(closePin, LOW);

  
}


void SmartCurtains::update() {


    bool circle = !digitalRead(top);//achieving top level detector

    if (circle and !circle_flag){
        counter++;
        circle_flag = 1;
    }
    if(!circle){
        circle_flag = 0;
    }
 

 if(counter == top_point){
    stop();
    counter = 0;
   }
   




}



