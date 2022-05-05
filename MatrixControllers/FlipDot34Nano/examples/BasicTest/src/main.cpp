#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include "flipDot34NanoGFX.h"

FlipDot34NanoGFX display(28, 19, 1, StackMode::kStacked);

void setup() {
  Serial.begin(115200);
  display.startWrite();
  display.fillScreen(0); 
  display.endWrite();
}

int color = 1;

void loop() {
  for (int y = 0; y < display.height(); y++) {
    for (int x = 0; x < display.width(); x++) {
      display.startWrite();
      display.drawPixel(x, y, color);
      display.endWrite();
      delay(5);
    }
  }
  delay(3000);
  color = 1 - color;
}
