#pragma once

#include <Arduino.h>
#include <SPI.h> // Adafruit_GFX needs this
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include "FlipDot34.h"

enum class StackMode {
    kRow,
    kStacked
};

class FlipDot34GFX : public GFXcanvas1, public FlipDot34  {
public:    
    FlipDot34GFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode);
    void setDotStart(int x, int y, bool state);
    void setDotEnd();
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
};
