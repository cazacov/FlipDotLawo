#include <Arduino.h>
#include "flipDot34SyncLib.h"

void setup() {
  // 2 columns of matrices, each 28x19 dots
  FlipDot32Sync.init(28, 19, 2);
  Serial.begin(115200);
}

bool state = false;

void loop() {

  for (int y = 0; y < 19; y++) {
    for (int x = 0; x < 56; x++) {
      FlipDot32Sync.setDot(x, y, state);    
    }
  }
  delay(3000);
  state = !state;
}