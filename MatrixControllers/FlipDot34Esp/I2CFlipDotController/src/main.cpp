#include <Arduino.h>
/*
#include <SPI.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include "FlipDot34GFX.h"
*/
#include <Wire.h>

// I2C Interface
#define SDA_PIN D2
#define SCL_PIN D1

const uint16_t I2C_ADDRESS = 0x08;

// Config pins
#define config0pin D5   
#define config1pin D6
#define config2pin D7

uint8_t controller_id;

// forward declarations
void receiveEvent(int howMany);

void setup() {
  Serial.begin(74880);
  delay(100); 

  /*
  // put your setup code here, to run once:
  pinMode(config0pin, INPUT_PULLUP);
  pinMode(config1pin, INPUT_PULLUP);
  pinMode(config2pin, INPUT_PULLUP);

  Serial.println();

  // Read configuration
  if (digitalRead(config0pin) == LOW) {
    controller_id |= 1;
  }
  if (digitalRead(config1pin) == LOW) {
    controller_id |= 2;
  }
  if (digitalRead(config2pin) == LOW) {
    controller_id |= 4;
  }
  Serial.print("Controller-ID: ");
  Serial.println(controller_id);
*/
  Wire.begin(SDA_PIN, SCL_PIN, I2C_ADDRESS); 
  Wire.setClock(25000L);
  //Wire.begin(sdaPin, sclPin, 0x40); 
  Wire.onReceive(receiveEvent);

  Serial.print("Listening on address ");
  Serial.println(I2C_ADDRESS);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print(".");
}

void receiveEvent(int howMany) {
  Serial.print("receive ");
  Serial.println(howMany);
  while(Wire.available()){
    uint8_t info = Wire.read();
    Serial.println(info);
  }
}