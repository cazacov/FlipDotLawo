#pragma once
#include <Arduino.h>
#include "flipDot34NanoGFX.h"

enum class GfxCommand {
    kNop = 0x00,
    kClear = 0x01,
    kFill = 0x02
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
public:
    CommandProcessor(Adafruit_GFX* display) : 
        display_(display)
    {
        command_ = GfxCommand::kNop;
        is_receiving_ = false;
        awaiting_bytes = 0;
        offset = 0;
    }
    void processByte(uint8_t next_byte);
};