#include "flipDot34GFX.h"

#define DISPLAY_WIDTH 28
#define DISPLAY_HEIGHT 19

// Three displays in a row
FlipDot34GFX display(DISPLAY_WIDTH, DISPLAY_HEIGHT, 3, StackMode::kRow);

void setup() {
  display.setRotation(0);
  display.fillScreen(0);
}

void loop() {
  display.startWrite();
  display.fillRect(0, 0, display.width(), 9, 1);
  display.setTextColor(0);

  for (int i = 0; i < 3; i++) {
    display.setCursor(DISPLAY_WIDTH * i + 11, 1);
    display.print(i+1);
  }
  
  display.setCursor(5,11);
  display.setTextColor(1);
  display.print("Hello World!");
  display.endWrite();
  delay(5000);
}
