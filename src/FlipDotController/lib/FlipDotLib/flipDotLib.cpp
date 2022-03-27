#include "flipDotLib.h"
#include "Arduino.h"
#include "TimerOne.h"

// 595 shift register
#define latchPin 9
#define dataPin 10
#define clockPin 11

#define enablePin 2

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

#define BUFFER_SIZE 532

// Library variables
int fd_width_;
int fd_height_;
volatile uint16_t buffer_[BUFFER_SIZE];
volatile size_t head_;
volatile size_t tail_;

// forward decalrations
void setEnable(uint8_t mask);
void sendZeros();
void sendData(uint16_t data);
void onTimer(void);

void init(int width, int height) {
    fd_width_ = width;
    fd_height_ = height;
    pinMode(enablePin, OUTPUT);
    pinMode(addr0pin, INPUT_PULLUP);
    pinMode(addr1pin, INPUT_PULLUP);
    pinMode(addr2pin, INPUT_PULLUP);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    setEnable(0);
    sendZeros();
//    Timer1.initialize(500);
    //Timer1.attachInterrupt(onTimer);
    tail_ = head_ = 0;
}

uint8_t addressMap[] = {
    1, 2, 3, 4, 5, 6, 7,
    9, 10, 11, 12, 13, 14, 15,
    17, 18, 19, 20, 21, 22, 23,
    25, 26, 27, 28, 29, 30, 31,
};

uint16_t calculateBits(int x, int y, bool state) {
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

void setDot(int x, int y, bool state) {

    uint16_t data = calculateBits(x, y, state);
    sendData(data);
    setEnable(1);
//    Serial.println(data, HEX);
    delayMicroseconds(300);
    setEnable(0);
    sendZeros();
    delayMicroseconds(300);
}


/*    
    if (tail_ + BUFFER_SIZE == head_ + BUFFER_SIZE + 1) {
        tail_++;
        tail_ %= BUFFER_SIZE;
    }
    buffer_[head_++] = result;
    head_ %= BUFFER_SIZE;
*/

/*

volatile bool timerFlip = false;
bool yDriverOn = false;

void onTimer(void)
{
    
  if (timerFlip) {
    if (head_ != tail_) {
        uint16_t data = buffer_[tail_];
        sendData(data);
        yDriverOn = true;
        setEnable(1);
        tail_++;
        tail_ %= BUFFER_SIZE;
    } else {
        if (yDriverOn) {
            sendZeros();
            yDriverOn = false;
        }
    }
  } else {
      setEnable(0);
  }
  timerFlip = !timerFlip;
}
*/

void setEnable(uint8_t mask) {
    mask <<= 2;
    PORTD |= mask;
    PORTD &= (mask + 3);
}

void sendZeros() {
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 16; i++) {
        bitClear(PORTB, clockPinPORTB); //clockOff();
        bitClear(PORTB, dataPinPORTB); //dataOff();
        bitSet(PORTB, clockPinPORTB); //clockOn();
    }
    digitalWrite(latchPin, HIGH);
}

void sendData(uint16_t data) {
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