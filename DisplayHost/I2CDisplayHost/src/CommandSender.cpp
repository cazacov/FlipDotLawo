#include "CommandSender.h"
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

void CommandSender::begin(uint8_t address) {
    address_ = address;
    Wire.begin(SDA_PIN, SCL_PIN);  // join i2c bus (address optional for master)
    Wire.setClock(100000L);
}

void CommandSender::clear_screen() {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kClear);              
    Wire.endTransmission();
    delay(1);
}

void CommandSender::fill_screen() {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kFill);              
    Wire.endTransmission();
    delay(1);
}

void CommandSender::set_color(uint8_t color) {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kSetColor);    
    Wire.write(1);
    Wire.endTransmission();
    delay(1);
}


void CommandSender::line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2) {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kLine);    
    Wire.write(x1);
    Wire.write(y1);
    Wire.write(x2);
    Wire.write(y2);
    Wire.endTransmission();
    delay(1);
}

void CommandSender::bitmap(uint8_t* bytes) {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kBitmap);          
    Wire.write(0);              
    Wire.write(0);
    Wire.write(28);
    Wire.write(19);

    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 4; j++) {
            int offset = i * 4 + j;
            Wire.write(bytes[offset]);
        }
        if (i % 5 == 4) {
            Wire.endTransmission();
            delay(10); 
            Wire.beginTransmission(address_); 
        }
    }
    Wire.endTransmission(); 
    //delay(15);
}

void CommandSender::setDelays(uint8_t percent) {
    Wire.beginTransmission(address_); 
    Wire.write((uint8_t)GfxCommand::kSetDelays);    
    Wire.write(percent);
    Wire.endTransmission();
    delay(1);
}