#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include "flipDot34NanoGFX.h"
#include "TimerOne.h"

FlipDot34NanoGFX display(28, 19, 1, StackMode::kRow);

const uint8_t STATE_IDLE = 0;
const uint8_t STATE_PULSE = 1;
const uint8_t STATE_DELAY = 2;
const long PULSE_MICROSECONDS = 300;
const long DELAY_MICROSECONDS = 200;

const int debugPin = 8;
uint8_t volatile state = STATE_IDLE;
uint8_t address;
char buf[40];

// forward
void receiveEvent(int howMany);
void timerCallback(); 

void setup() {
  Serial.begin(115200);

  address = 0x20 + display.readConfig();
  display.clearScreen();
  sprintf(buf, "0x%02X", address * 2);
  display.printCentered(buf, display.width()/2, display.height()/2);
  display.endWrite();
  
  pinMode(debugPin, OUTPUT);
  digitalWrite(debugPin, LOW);

  Wire.begin(address);
  Wire.onReceive(receiveEvent);
  Serial.print("Listening on address: "); 
  Serial.print(address * 2, HEX);
  delay(2000);
  
  // test 
  for (int16_t i = 0; i < display.width(); i++) {
    display.writeFastVLine(i, 0, display.height(), 1);
  }

  Timer1.initialize();
  Timer1.attachInterrupt(timerCallback);
  Timer1.stop();
}

void loop() {
  if (state != STATE_IDLE) {
    return;
  }

  while (!display.updateNext()) {
    ; // do nothing
  }
  digitalWrite(debugPin, HIGH);
  Timer1.setPeriod(PULSE_MICROSECONDS);
  state = STATE_PULSE;
}

void receiveEvent(int howMany)
{
  while( Wire.available()) // loop through all but the last
  {
    char c = Wire.read(); // receive byte as a character
    Serial.print(c, HEX);         // print the character
  }
  Serial.println();         // print the integer
}

void timerCallback() {
  Timer1.stop();

  if (state == STATE_PULSE) {
    state = STATE_DELAY;
    display.endPulse();
    digitalWrite(debugPin, LOW);
    Timer1.setPeriod(DELAY_MICROSECONDS);
  } else if (state == STATE_DELAY) {
      state = STATE_IDLE;
  }
}