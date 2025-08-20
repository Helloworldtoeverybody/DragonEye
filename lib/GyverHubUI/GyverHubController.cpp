#include "GyverHubController.h"
#include <Arduino.h>
#include <Smartlight.h>
#include<GyverHub.h>
#include "Autowattering.h"
#include <FanController.h>

extern FanController fanController;
extern Smartlight lamp;

extern Wattering plant;
 byte temp_number = 0;


void btn_cb_Fireflies(){
     lamp.setEffect(0);
}



void btn_cb_Meteor(){
     lamp.setEffect(1);
}
void btn_cb_fire() {
       
   

}

void btn_cb_hello() {
       
    lamp.setEffect(2);

}
void btn_cb_rainbow() {
       
    lamp.setEffect(3);
    Serial.println("Effect 3");

}
void btn_cb_sky() {
       
    lamp.setEffect(4);
    Serial.println("Effect 4");

}


void btn_cb_off() {
       
    lamp.setEffect(5);
    Serial.println("Effect off");

}

void btn_cb_white() {
       
    if (temp_number == 8) temp_number = 0;
    else{temp_number++;}
    
    Serial.println(temp_number);
    
    lamp.setEffect(6);
    lamp.setTemperature(temp_number);
    Serial.printf("Effect white_temperature");

}

void btn_cb_waterring(){
    
    plant.plant_flag = 1;
}







// Основной метод сборки интерфейса
void GyverHubUI::build(gh::Builder& b) {
    static byte tab;
    if (b.Tabs(&tab).text("Lamp;Curtains;Plant;System").click()) b.refresh(); // обновить по клику

    switch(tab){
        case 0:
            b.Title("Lamp").color(gh::Colors::Cream);
            buildBrightnessSlider(b);
            buildColorButtonsRow1(b);
            buildColorButtonsRow2(b);
            
            break;
        case 1:
            b.Title("Curtains").color(gh::Colors::Cream);

            

            break;
        case 2:
            b.Title("Plant").color(gh::Colors::Cream);
            buildWatterController(b);

            break;
        case 3:
        //Тут можно накидать вывод в телефон всех данныз из serial
            b.Title("System").color(gh::Colors::Cream);
            buildFan(b);

            break;
        default:
            buildColorButtonsRow1(b);
            buildColorButtonsRow2(b);
            buildBrightnessSlider(b);
        
    }

}







void GyverHubUI::buildBrightnessSlider(gh::Builder& b) {
    if (b.Slider_("bright", &bright).click()) {
        Serial.print("bright set to: ");
        Serial.println(b.build.value);
        lamp.setBrightness(bright);
    }
}

void GyverHubUI::buildPowerSlider(gh::Builder& b) {
    if (b.Slider_("power", &power).click()) {
        power = map(power, 0, 100, 0, 1023);
        Serial.print("power set to: ");
        Serial.println(b.build.value);
    }
}

void GyverHubUI::buildHueSlider(gh::Builder& b) {
    if (b.Slider_("hue", &hue).click()) {
        hue = map(hue, 0, 100, 0, 255);
        Serial.print("hue set to: ");
        Serial.println(hue);
    }
}

void GyverHubUI::buildColorButtonsRow1(gh::Builder& b) {
    gh::Row r(b, 1);
    b.Button().attach(btn_cb_Meteor).color(gh::Colors::Brown).icon("f3ed");
    b.Button().attach(btn_cb_white).color(gh::Colors::Yellow).icon("f185");
    b.Button().attach(btn_cb_Fireflies).color(gh::Colors::Cream).icon("f0c2");
    b.Button().attach(btn_cb_rainbow).color(gh::Colors::Brown).icon("f75b");
}


void GyverHubUI::buildColorButtonsRow2(gh::Builder& b) {
        gh::Row r(b, 1);
    b.Button().attach(btn_cb_fire).color(gh::Colors::Yellow).icon("f7e4");
    b.Button().attach(btn_cb_hello).color(gh::Colors::Cream).icon("f714");
    b.Button().attach(btn_cb_off).color(gh::Colors::Red).icon("f00d");
    
    b.Button().attach(btn_cb_rainbow).color(gh::Colors::Obsidian).icon("f074");
}








void GyverHubUI::buildWatterController(gh::Builder& b) {
    



            
        gh::Row r(b);
        
        {
            gh::Col c(b);
            b.Button().attach(btn_cb_waterring).color(gh::Colors::Yellow).icon("f73d");
            b.Spinner_("Hours", &plant_hour).range(0, 23, 1);

            b.Spinner_("Delay", &plant_delay).range(0, 100, 1);
        }
         
        {
            gh::Col c(b);
           for (int i = 0; i < 7; i++) {
            if (b.Flags().text(days[i]).size(3).color(gh::Colors::Yellow).click()) {
                
                
                flags[i] = !flags[i];
                plant.updateFlags(days[i], flags[i], plant_hour, plant_delay);
        }
    }

        }
}



void GyverHubUI::buildFan(gh::Builder& b) {
    if (b.Slider_("Fan_brightness", &fan_power).click()) {
        fan_power = map(fan_power, 0, 100, 0, 1023);
        fanController.setFanSpeed(fan_power, 1);
        Serial.print("Fan speed set to: ");
        Serial.println(fan_power);
    }
    b.Switch_("Auto", &fanController.auto_fan).color(gh::Colors::Yellow).icon("f73d");
}