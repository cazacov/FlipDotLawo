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
    offset = 0;
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
        case (int)GfxCommand::kDotSet:
            command_ = GfxCommand::kDotSet;
            awaiting_bytes = 2;
            return true;
            break;
        case (int)GfxCommand::kDotReset:
            command_ = GfxCommand::kDotReset;
            awaiting_bytes = 2;
            return true;
            break;
        case (int)GfxCommand::kSetColor:
            command_ = GfxCommand::kSetColor;
            awaiting_bytes = 1;
            return true;
            break;
        case (int)GfxCommand::kLine:
            command_ = GfxCommand::kLine;
            awaiting_bytes = 4;
            return true;
            break;         
        case (int)GfxCommand::kRect:
            command_ = GfxCommand::kRect;
            awaiting_bytes = 4;
            return true;
            break;               
        case (int)GfxCommand::kFillRect:
            command_ = GfxCommand::kFillRect;
            awaiting_bytes = 4;
            return true;
            break;          
        case (int)GfxCommand::kCircle:
            command_ = GfxCommand::kCircle;
            awaiting_bytes = 3;
            return true;
            break;               
        case (int)GfxCommand::kFillCircle:
            command_ = GfxCommand::kFillCircle;
            awaiting_bytes = 3;
            return true;
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
        case GfxCommand::kDotSet:
            display_->drawPixel(buffer[0], buffer[1], 1);
            break;
        case GfxCommand::kDotReset:
            display_->drawPixel(buffer[0], buffer[1], 0);
            break;       
        case GfxCommand::kSetColor:
            color = buffer[0];
            break;
        case GfxCommand::kLine:
            display_->drawLine(buffer[0], buffer[1], buffer[2], buffer[3], color);
            break;    
        case GfxCommand::kRect:
            display_->drawRect(buffer[0], buffer[1], buffer[2], buffer[3], color);
            break;         
        case GfxCommand::kFillRect:
            display_->fillRect(buffer[0], buffer[1], buffer[2], buffer[3], color);
            break;   
        case GfxCommand::kCircle:
            display_->drawCircle(buffer[0], buffer[1], buffer[2], color);
            break;
        case GfxCommand::kFillCircle:
            display_->fillCircle(buffer[0], buffer[1], buffer[2], color);
            break;
        default:
            // Do nothing;
            break;
    }   
}