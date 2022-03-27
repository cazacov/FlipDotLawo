#include <Arduino.h>
#include "flipDotLib.h"

void setup() {
  // put your setup code here, to run once:
  init(28, 19);
  Serial.begin(115200);
  setDot(5,5, true);
}

bool state = false;

void loop() {

  for (int y = 0; y < 19; y++) {
    for (int x = 0; x < 28; x++) {
      setDot(x, y, state);    
    }
  }
  delay(3000);
  state = !state;
}