#include <Arduino.h>
#include <Wire.h>
#include "frames56x38.h"
#include "CommandSender.h"

const int senders_count = 4;
const int play_every = 1;
const int frame_count = 1158;

const uint8_t addresses[senders_count] = {
  0x20,
  0x21,
  0x22,
  0x23
};

CommandSender senders[senders_count];

//const int frame_count = sizeof(frames38x28) / sizeof(frames38x28[0]);

void setup()
{
  // put your setup code here, to run once:

  //Wire.begin(sdaPin, sclPin );
  //Wire.setClock(400000);
  Serial.begin(115200);
  delay(5000);

  for (int i = 0; i < senders_count; i++) {
    senders[i].begin(addresses[i]);
  }

  for (int i = 0; i < senders_count; i++) {
    senders[i].setDelays(100);
    senders[i].fill_screen();
    delay(500);
    senders[i].clear_screen();
    delay(500);
  }
  delay(1000);
}

uint16_t frame_nr = 0;

void loop()
{

  uint8_t buf[80];

  long ms = millis();
  for (int sender = 0; sender < senders_count; sender++) {
    for (int i = 0; i < 76; i++)
    {
      buf[i] = pgm_read_byte(&(frames56x38[frame_nr][sender][i]));
    }
    senders[sender].bitmap(buf);
  }
  frame_nr += play_every;
  if (frame_nr >= frame_count)
  {
    frame_nr = 0;
  }
//  Serial.println(frame_nr);
  int frame_length = 1000 * play_every / 5;

  long left = frame_length + ms - millis();
  Serial.println(left);
  if (left > 0)
  {
    delay(left);
  }
  else
  {
    delay(2);
  }

  return;
}

/*
void sendFrame(int frame_nr) {
  Wire.beginTransmission(I2C_ADDRESS); 
  Wire.write(0x89);              
  Wire.write(0);              
  Wire.write(0);
  Wire.write(28);
  Wire.write(19);

  for (int i = 0; i < 19; i++) {
    for (int j = 0; j < 4; j++) {

      int offset = i * 4 + j;

      Wire.write(pgm_read_byte(&(frames[frame_nr][offset])));
    }
    if (i % 5 == 4) {
      Wire.endTransmission();
      delay(80); 
      Wire.beginTransmission(I2C_ADDRESS); 
    }
  }
  Wire.endTransmission(); 
  Serial.println(frame_nr);  
}
*/