#pragma once

#include <Arduino.h>
#include <SPI.h> // Adafruit_GFX needs this
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>

enum class StackMode {
    kRow,
    kStacked
};

class FlipDot34NanoGFX : public GFXcanvas1 {
public:    
    FlipDot34NanoGFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode);
    void setDot(int x, int y, bool state);
    void clearScreen();
    uint8_t readConfig();
    void setBitmap(const uint8_t *bytes, size_t count);
    bool updateNext();
// Override AdafruitGFX
    void endWrite(void);
// GFX Helper    
    void printCentered(const char *buf, uint16_t x, uint16_t y);
protected:
    int matrix_width_;
    int matrix_height_;
    int display_width_;
    int display_height_;
    int matrix_columns_;
    bool is_stacked_;
    uint8_t* volatile frameBuffer;
    uint16_t frameBufferSize;
    uint16_t frameBufferWidth;
    uint8_t next_x;
    uint8_t next_y;
    uint8_t next_mask;
    uint8_t next_offset;

    void setDotStart(int x, int y, bool state);
    void sendZeros();
    void sendData(uint8_t x, uint8_t y, uint8_t matrix, bool state);
    void sendDataRaw(uint32_t data); 
};
