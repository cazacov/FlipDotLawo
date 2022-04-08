#pragma once

#include <Arduino.h>
#include <SPI.h> // Adafruit_GFX needs this
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>

enum class StackMode {
    kRow,
    kStacked
};

class FlipDot34GFX : public GFXcanvas1 {
public:    
    FlipDot34GFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode);
    void setDot(int x, int y, bool state);
    void clearScreen();
// Override AdafruitGFX
    void endWrite(void);
protected:
    int matrix_width_;
    int matrix_height_;
    int display_width_;
    int display_height_;
    int matrix_columns_;
    bool is_stacked_;
    uint8_t* frameBuffer;
    uint16_t frameBufferSize;
    uint16_t frameBufferWidth;
    uint8_t addressMap[28] = {
        1, 2, 3, 4, 5, 6, 7,
        9, 10, 11, 12, 13, 14, 15,
        17, 18, 19, 20, 21, 22, 23,
        25, 26, 27, 28, 29, 30, 31,
    };
    uint16_t calculateBits(uint8_t x, uint8_t y, bool state);
    void setEnable(uint8_t mask);
    void sendZeros();
    void sendData(uint16_t data);
};
