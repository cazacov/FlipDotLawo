#include "FlipDot34.h"
#include "Arduino.h"

// 595 shift register
#define dataPin D4   
#define latchPin D3
#define clockPin D8

FlipDot34::FlipDot34()
{
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    sendZeros();
}

// 2800 address lines when acessed over 74hc595 have bit order: (high bit) B1, B0, A2, A1, A0 (low bit)
uint8_t addressMap[28] = {
    16, 8, 24, 4, 20, 12, 28,
    18, 10, 26, 6, 22, 14, 30,
    17, 9, 25, 5, 21, 13, 29,
    19, 11, 27, 7, 23, 15, 31};

void FlipDot34::sendData(uint8_t x, uint8_t y, uint8_t column, bool state)
{
    x = addressMap[x];
    y = addressMap[y];
    
    uint16_t addr = x;
    addr <<= 8;
    if (x & 0x01)
    {
        addr |= 0x80;
    }
    addr |= (y << 2);

    // Set X DATA
    if (state)
    {
        addr |= 0x2000;
    }
    // Set Y ENABLE
    if (state)
    {
        addr |= 0x4000;
    }
    else
    {
        addr |= 0x8000;
    }

    uint8_t enable = 0x01 << column;

    sendDataRaw(addr, enable);
}

void FlipDot34::sendZeros()
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    shiftOut(dataPin, clockPin, MSBFIRST, 0);
    digitalWrite(latchPin, HIGH);
}

void FlipDot34::sendDataRaw(uint16_t address, uint8_t enable)
{
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, (uint8_t)(address >> 8));
    shiftOut(dataPin, clockPin, MSBFIRST, (uint8_t)(address & 0x00FF));
    shiftOut(dataPin, clockPin, MSBFIRST, enable);
    digitalWrite(latchPin, HIGH);
}