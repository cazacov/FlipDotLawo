#include "flipDot34SyncLib.h"
#include "Arduino.h"

// 595 shift register
#define latchPin 9
#define dataPin 10
#define clockPin 11

#define enableFirstPin 2
#define enablePinCount 6

// Address jumpers
#define addr0pin 8
#define addr1pin 12
#define addr2pin 13

// 595 helpers
#define latchPinPORTB 1  //int latchPinPORTB = latchPin - 8;
#define clockPinPORTB 3  //int clockPinPORTB = clockPin - 8;
#define dataPinPORTB  2  //int dataPinPORTB = dataPin - 8;

// Shift register bits
// bits 1-5 Y-Driver A0-B1
// bit  8 - X-Driver Data
// bit  9 - Y-Enable-1
// bit  10 - Y-Enable-2
// bits 11-15 X-Driver A0-B1
#define dataBit 1 << 8
#define enableOnBit 1 << 10
#define enableOffBit 1 << 9

#define pulseLengthMicroseconds 200
#define pauseLengthMicroseconds 10

FlipDot32Sync_ FlipDot32Sync;

void FlipDot32Sync_::init(int matrix_width, int matrix_height, int matrix_columns) {
    matrix_width_ = matrix_width;
    matrix_height_ = matrix_height;
    matrix_columns_ = matrix_columns;
    display_width_ = matrix_width * matrix_columns;
    display_height_ = matrix_height;
    for (uint8_t pin = enableFirstPin; pin < enableFirstPin + enablePinCount; pin++) {
        pinMode(pin, OUTPUT);
    }
    pinMode(addr0pin, INPUT_PULLUP);
    pinMode(addr1pin, INPUT_PULLUP);
    pinMode(addr2pin, INPUT_PULLUP);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    setEnable(0);
    sendZeros();
}

uint16_t FlipDot32Sync_::calculateBits(uint8_t x, uint8_t y, bool state) {
    x = addressMap[x];
    y = addressMap[y];

    uint16_t result = x;
    result <<= 11;
    result |= (y << 1);

    // Set X DATA
    if (state) {
        result |= dataBit;
    }
    // Set Y ENABLE
    if (state) {
        result |= enableOnBit;
    } else {
        result |= enableOffBit;
    }
    return result;
}

void FlipDot32Sync_::setDot(int x, int y, bool state) {
   
    if (x >= display_width_) {
        return;
    }
    if (y >= display_height_) {
        return;
    }
    
    uint8_t matrix_column = 0;
    while (x >= matrix_width_) {
        x -= matrix_width_;
        matrix_column++;
    }
    
    uint16_t data = calculateBits(x, y, state);
    sendData(data);
    setEnable(1 << matrix_column);
    delayMicroseconds(pulseLengthMicroseconds);
    setEnable(0);
    sendZeros();
    delayMicroseconds(pauseLengthMicroseconds);
}

void FlipDot32Sync_::setEnable(uint8_t mask) {
    mask <<= 2;
    PORTD |= mask;
    PORTD &= (mask + 3);
}

void FlipDot32Sync_::sendZeros() {
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 16; i++) {
        bitClear(PORTB, clockPinPORTB); //clockOff();
        bitClear(PORTB, dataPinPORTB); //dataOff();
        bitSet(PORTB, clockPinPORTB); //clockOn();
    }
    digitalWrite(latchPin, HIGH);
}

void FlipDot32Sync_::sendData(uint16_t data) {
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < 16; i++)  {
    bitClear(PORTB, clockPinPORTB); //clockOff();
    //--- Turn data on or off based on value of bit
    if (data & 0x8000) {
      bitSet(PORTB, dataPinPORTB); // dataOn();
    }
    else {
      bitClear(PORTB, dataPinPORTB); //dataOff();
    }
    data <<= 1;
    //register shifts bits on upstroke of clock pin
    bitSet(PORTB, clockPinPORTB); //clockOn();
  }
  digitalWrite(latchPin, HIGH);
}