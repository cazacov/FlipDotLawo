#include <Arduino.h>
#include "flipDot34GFX.h"


FlipDot34GFX display(28, 19, 1, StackMode::kStacked);

void setup() {
  // 2 columns of matrices, each 28x19 dots
  Serial.begin(115200);


  display.setRotation(3);
  display.fillScreen(0);
  display.setTextColor(1,0);
  display.setCursor(1, 1);
  display.print("Hi");
  delay(5000);
}

bool state = false;

void loop() {

  for (int y = 0; y < 8; y++) {
    for (int x = 0; x < 12; x++) {
      display.setDot(x, y, state);    
    }
  }
  delay(3000);
  state = !state;
}