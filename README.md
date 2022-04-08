# FlipDot34

Controller for the flip-dot display with 34-pin connector. For example produced by Lawo-Luminator-Europa Mark IV Industries GmbH.

## Connector Pinout
| Pin |  Function | Pin |  Function |
| --- | ------ | --- | ------ |
| 1   | GND  | 2   | GND  | 
| 3   | GND  | 4   | GND  | 
| 5   | 5V  |  6   | 5V  | 
| 7   | A0-X | 8 | DATA-X |
| 9   | A1-X | 10 | ENABLE-Y-ON |
| 11   | A2-X | 12 | ENABLE-Y-OFF |
| 13   | B0-X | 14 | ENABLE-X-1 |
| 15   | B1-X | 16 | ENABLE-X-2 |
| 17   | A0-Y | 18 | ENABLE-X-3 |
| 19   | A1-Y | 20 | ENABLE-X-4 |
| 21   | A2-Y | 22 | ENABLE-X-5 |
| 23   | B0-Y | 24 | ENABLE-X-6 |
| 25   | B1-Y | 26 | ENABLE-X-7 |
| 27   | n/c | 28 | ENABLE-X-8 |
| 29   | 24V  |  30   | 24V  | 
| 31   | GND  | 32   | GND  | 
| 33   | GND  | 34   | GND  | 

## Controller Prototype 

The prototype uses Arduino Nano and two 74HC595 shift registers to control up to 6 matrices. By adding another 595 it can control up to 8 matrices.

![Prototype](./_img/FlipDot34-prototype.png)

## Display Prototype



---
All trademarks, logos and brand names are the property of their respective owners. All company, product and service names are for identification purposes only. 

It's a hobby project; I am not affiliated with any  company producing flip-dot displays.

