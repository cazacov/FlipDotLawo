#pragma once

#include <Arduino.h>

class FlipDot34 {
public:    
    FlipDot34();
protected:
    void sendZeros();
    void sendData(uint8_t x, uint8_t y, uint8_t column, bool state);
    void sendDataRaw(uint16_t address, uint8_t enable);
};
