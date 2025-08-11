#include <Smartcurtain.h>
#include<Arduino.h>




SmartCurtains::SmartCurtains(int openPin, int closePin, int b_openPin, int b_closePin, int top)
: openPin(openPin), closePin(closePin), b_openPin(b_openPin),  b_closePin(b_closePin), top(top), targetPosition(0), currentPosition(0), isMoving(false), lastUpdate(0)
{}



void SmartCurtains::begin(){


    pinMode(openPin, OUTPUT);
    pinMode(closePin, OUTPUT);
    
    pinMode(b_openPin, OUTPUT);
    pinMode(b_closePin, OUTPUT);

    pinMode(top, INPUT);

}


void SmartCurtains::open() {
    /*
    digitalWrite(openPin, HIGH);
    digitalWrite(closePin, LOW);
    isMoving = true;
*/
  

    digitalWrite(openPin, HIGH);
    digitalWrite(closePin, LOW);

    analogWrite(b_openPin, 255);
    analogWrite(b_closePin, 0);
    //openning till achieving highest level

}

void SmartCurtains::close() {
    digitalWrite(openPin, LOW);
    digitalWrite(closePin, HIGH);

    analogWrite(b_openPin, 0);
    analogWrite(b_closePin, 155);
    isMoving = true;

}



void SmartCurtains::stop() {
    digitalWrite(openPin, LOW);
    digitalWrite(closePin, LOW);

    analogWrite(b_openPin, 0);
    analogWrite(b_closePin, 0);
    
    isMoving = false;
}


void SmartCurtains::setPosition(int pos) {
    pos = constrain(pos, 0, 100);
    if (pos > currentPosition) {
        open();
    } else if (pos < currentPosition) {
        close();
    }
    targetPosition = pos;
}

void SmartCurtains::update() {

    top_point = digitalRead(top);//achieving top level detector

if(!top_point){
    top_flag = 1;
    
}

if(top_flag){
    stop();
    top_flag = 0;
}

    /*
    if (!isMoving) return;  // Если шторы не двигаются — просто выходим

    // Проверяем время, чтобы делать обновление не слишком часто
    if (millis() - lastUpdate > 100) {  // Обновляем раз в 100 мс
        lastUpdate = millis();

        if (currentPosition < targetPosition) {
            currentPosition++;  // Шторы немного открылись
        } else if (currentPosition > targetPosition) {
            currentPosition--;  // Шторы немного закрылись
        }

        // Если достигли цели — останавливаем движение
        if (currentPosition == targetPosition) {
            stop(); q
        }
    }
    */

}


