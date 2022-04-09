#include "flipDot34GFX.h"

#define DISPLAY_WIDTH 28
#define DISPLAY_HEIGHT 19

// A simplest "row" having only one display
FlipDot34GFX display(DISPLAY_WIDTH, DISPLAY_HEIGHT, 1, StackMode::kRow);

void setup() {
  display.setRotation(0);
  display.fillScreen(0);
}

void loop() {
  // Draw a grid of 5x5 cells
  display.startWrite();
  for (int x = 0; x < DISPLAY_WIDTH; x += 5) {
    display.drawLine(x, 0, x, DISPLAY_HEIGHT - 1, 1);
  }
  for (int y = 0; y < DISPLAY_HEIGHT; y += 5) {
    display.drawLine(0, y,DISPLAY_WIDTH-1, y, 1);
  }
  display.endWrite();
}
