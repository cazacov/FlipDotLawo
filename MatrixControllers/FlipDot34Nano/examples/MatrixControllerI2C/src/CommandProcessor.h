#pragma once
#include <Arduino.h>
#include "flipDot34NanoGFX.h"

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
};

class CommandProcessor {
private:
    FlipDot34NanoGFX* display_;
    bool is_receiving_;
    size_t awaiting_bytes;
    size_t offset; 
    GfxCommand command_;
    uint8_t buffer[160];
    void got_new_command(uint8_t next_byte);
    void executeCommand();
    bool decodeCommand(uint8_t data);
    void copyBitmap(uint8_t x0, uint8_t y0, uint8_t w, uint8_t h, uint8_t *bytes);

    uint8_t color;
public:
    CommandProcessor(FlipDot34NanoGFX* display) : 
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