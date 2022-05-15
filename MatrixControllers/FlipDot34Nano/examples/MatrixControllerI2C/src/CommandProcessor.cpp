#include "CommandProcessor.h"

void CommandProcessor::processByte(uint8_t next_byte) {
    if (is_receiving_) {
        buffer[offset++] = next_byte;
        awaiting_bytes--;
        if (awaiting_bytes == 0) {
            is_receiving_ = false;
            executeCommand();
        }
    } else {
        is_receiving_ = decodeCommand(next_byte);
        if (!is_receiving_) {   // Self-contained command
            executeCommand();
        }
    }
}

bool CommandProcessor::decodeCommand(uint8_t data) {
    switch (data) {
        case (int)GfxCommand::kClear:
            command_ = GfxCommand::kClear;
            awaiting_bytes = 0;
            return false;
            break;
        case (int)GfxCommand::kFill:
            command_ = GfxCommand::kFill;
            awaiting_bytes = 0;
            return false;
            break;
        default:
            command_ = GfxCommand::kNop;
            awaiting_bytes = 0;
            return false;
    }
    return true;
}

void CommandProcessor::executeCommand() {
    switch (command_) {
        case GfxCommand::kClear:
            for (int16_t y = 0; y < display_->height(); y++) {
                display_->writeFastHLine(0, y, display_->width(), 0);
            }
            break;
        case GfxCommand::kFill:
            for (int16_t y = 0; y < display_->height(); y++) {
                display_->writeFastHLine(0, y, display_->width(), 1);
            }
            break;
        default:
            // Do nothing;
            break;
    }   
}