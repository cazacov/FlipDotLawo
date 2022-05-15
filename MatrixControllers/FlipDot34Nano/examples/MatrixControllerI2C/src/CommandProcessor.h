#pragma once
#include <Arduino.h>
#include "flipDot34NanoGFX.h"

enum class GfxCommand {
    kNop = 0x00,
    kClear = 0x01,
    kFill = 0x02,
    kDotSet = 0x03,
    kDotReset = 0x04,
    kSetColor = 0x05,
    kLine = 0x06,
    kRect = 0x07,
    kFillRect = 0x08,
    kCircle = 0x09,
    kFillCircle = 0x0A
};

class CommandProcessor {
private:
    Adafruit_GFX* display_;
    bool is_receiving_;
    size_t awaiting_bytes;
    size_t offset; 
    GfxCommand command_;
    uint8_t buffer[80];
    void executeCommand();
    bool decodeCommand(uint8_t data);

    uint8_t color;
public:
    CommandProcessor(Adafruit_GFX* display) : 
        display_(display)
    {
        command_ = GfxCommand::kNop;
        is_receiving_ = false;
        awaiting_bytes = 0;
        offset = 0;
        color = 1;
    }
    void processByte(uint8_t next_byte);
};