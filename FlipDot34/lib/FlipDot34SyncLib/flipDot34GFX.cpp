#include "FlipDot34GFX.h"
#include "Arduino.h"

// 595 shift register
#define latchPin 9
#define dataPin 10
#define clockPin 11

#define enableFirstPin 2
#define enablePinCount 6

// Address jumpers
#define addr0pin 8
#define addr1pin 12
#define addr2pin 13

// 595 helpers
#define latchPinPORTB 1  //int latchPinPORTB = latchPin - 8;
#define clockPinPORTB 3  //int clockPinPORTB = clockPin - 8;
#define dataPinPORTB  2  //int dataPinPORTB = dataPin - 8;

// Shift register bits
// bits 1-5 Y-Driver A0-B1
// bit  8 - X-Driver Data
// bit  9 - Y-Enable-1
// bit  10 - Y-Enable-2
// bits 11-15 X-Driver A0-B1
#define dataBit 1 << 8
#define enableOnBit 1 << 10
#define enableOffBit 1 << 9

#define pulseLengthMicroseconds 200
#define pauseLengthMicroseconds 100

FlipDot34GFX::FlipDot34GFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode)
    : GFXcanvas1(matrix_width * matrix_columns, stack_mode == StackMode::kRow ? matrix_height : matrix_height * 2)
 {
    is_stacked_ = stack_mode == StackMode::kStacked;

    matrix_width_ = matrix_width;
    matrix_height_ = matrix_height;
    matrix_columns_ = matrix_columns;
    display_width_ = matrix_width * matrix_columns;
    display_height_ = matrix_height;
    frameBufferWidth = (WIDTH + 7) >> 3;
    frameBufferSize = frameBufferWidth * HEIGHT;
    frameBuffer = (uint8_t*) malloc(frameBufferSize);
    memset(frameBuffer, 0, frameBufferSize);
    for (uint8_t pin = enableFirstPin; pin < enableFirstPin + enablePinCount; pin++) {
        pinMode(pin, OUTPUT);
    }
    pinMode(addr0pin, INPUT_PULLUP);
    pinMode(addr1pin, INPUT_PULLUP);
    pinMode(addr2pin, INPUT_PULLUP);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    setEnable(0);
    sendZeros();

    clearScreen();
}

uint16_t FlipDot34GFX::calculateBits(uint8_t x, uint8_t y, bool state) {
    x = addressMap[x];

    if (!is_stacked_) {
        y = addressMap[y];
    }
    else {
        if (y < matrix_height_) {
            y = addressMap[matrix_height_ - y - 1];
        }
        else {
            y = addressMap[y - matrix_height_];
        }
    }

    uint16_t result = x;
    result <<= 11;
    result |= (y << 1);

    // Set X DATA
    if (state) {
        result |= dataBit;
    }
    // Set Y ENABLE
    if (state) {
        result |= enableOnBit;
    } else {
        result |= enableOffBit;
    }
    return result;
}

void FlipDot34GFX::setDot(int x, int y, bool state) {
   
    if ((x < 0) || (y < 0) || (x >= WIDTH) || (y >= HEIGHT))
      return;
    
    uint8_t matrix_column = 0;
    while (x >= matrix_width_) {
        x -= matrix_width_;
        matrix_column++;
    }
    if (is_stacked_) {
        if (y < matrix_height_) {
            matrix_column <<= 1;
            matrix_column++;
            x = matrix_width_ - x - 1;
        }
        else {
            matrix_column <<= 1;
        }
    }
    
    uint16_t data = calculateBits(x, y, state);
    sendData(data);
    setEnable(1 << matrix_column);
    delayMicroseconds(pulseLengthMicroseconds);
    setEnable(0);
    sendZeros();
    delayMicroseconds(pauseLengthMicroseconds);
}

void FlipDot34GFX::setEnable(uint8_t mask) {
    mask <<= 2;
    PORTD |= mask;
    PORTD &= (mask + 3);
}

void FlipDot34GFX::sendZeros() {
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 16; i++) {
        bitClear(PORTB, clockPinPORTB); //clockOff();
        bitClear(PORTB, dataPinPORTB); //dataOff();
        bitSet(PORTB, clockPinPORTB); //clockOn();
    }
    digitalWrite(latchPin, HIGH);
}

void FlipDot34GFX::sendData(uint16_t data) {
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < 16; i++)  {
    bitClear(PORTB, clockPinPORTB); //clockOff();
    //--- Turn data on or off based on value of bit
    if (data & 0x8000) {
      bitSet(PORTB, dataPinPORTB); // dataOn();
    }
    else {
      bitClear(PORTB, dataPinPORTB); //dataOff();
    }
    data <<= 1;
    //register shifts bits on upstroke of clock pin
    bitSet(PORTB, clockPinPORTB); //clockOn();
  }
  digitalWrite(latchPin, HIGH);
}

void FlipDot34GFX::clearScreen() {
    for (int16_t y = 0; y < HEIGHT; y++) {
        for (int16_t x = 0; x < WIDTH; x++) {
            setDot(x,y,false);
        }
    }
}

// overwrites Adafruit
void FlipDot34GFX::endWrite(void) {

    uint8_t* buf_ptr = frameBuffer;
    uint8_t* canvas_ptr = getBuffer();

    Serial.print("Framebuffer width:");
    Serial.println(frameBufferWidth);


    for(int16_t y = 0; y < HEIGHT; y++) {
        uint8_t mask = 0x80;   
        uint8_t buf_val = *buf_ptr;
        uint8_t canvas_val = *canvas_ptr;
        for(int16_t x = 0; x < WIDTH; x++) {
            if ((buf_val ^ canvas_val) & mask) {
                setDot(x, y, canvas_val & mask);
            }
            mask >>= 1;
            if (!mask) {
                mask = 0x80;
                buf_ptr++;
                canvas_ptr++;
                buf_val = *buf_ptr;
                canvas_val = *canvas_ptr;
            }
        }
        if (WIDTH & 0x07) {
            buf_ptr++;
            canvas_ptr++;
        }    
    }
    //store previous frame Buffer
    memcpy(frameBuffer, getBuffer(), frameBufferSize);
}