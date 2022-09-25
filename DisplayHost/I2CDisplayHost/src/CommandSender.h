#pragma once

#include <Arduino.h>

#define COMMAND_FLAG 0x80

enum class GfxCommand {
    kNop = COMMAND_FLAG,
    kClear = 1 | COMMAND_FLAG,
    kFill = 2 | COMMAND_FLAG,
    kDotSet = 3 | COMMAND_FLAG,
    kDotReset = 4 | COMMAND_FLAG,
    kSetColor = 5 | COMMAND_FLAG,
    kLine = 6 | COMMAND_FLAG,
    kRect = 7 | COMMAND_FLAG,
    kFillRect = 8 | COMMAND_FLAG,
    kBitmap = 9 | COMMAND_FLAG,
    kSetDelays = 10 | COMMAND_FLAG,    
};

class CommandSender {
private:
    uint8_t address_;    
public:
    void begin(uint8_t address);
    void clear_screen();
    void fill_screen();
    void set_color(uint8_t color);
    void line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
    void bitmap(uint8_t* bytes);
    void setDelays(uint8_t percent);
};