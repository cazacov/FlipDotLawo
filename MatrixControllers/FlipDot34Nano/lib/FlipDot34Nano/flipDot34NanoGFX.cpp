#include "FlipDot34NanoGFX.h"
#include "Arduino.h"

// 595 shift register
#define dataPin 3   
#define latchPin 5
#define clockPin 6

// Address jumpers
#define addr0pin A0
#define addr1pin A1
#define addr2pin A2

#define pulseLengthMicroseconds 200
#define pauseLengthMicroseconds 100

FlipDot34NanoGFX::FlipDot34NanoGFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode)
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
    frameBuffer = (uint8_t *)malloc(frameBufferSize);
    memset(frameBuffer, 0, frameBufferSize);
    pinMode(addr0pin, INPUT_PULLUP);
    pinMode(addr1pin, INPUT_PULLUP);
    pinMode(addr2pin, INPUT_PULLUP);
    pinMode(latchPin, OUTPUT);
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    sendZeros();
    clearScreen();
}

void FlipDot34NanoGFX::setDot(int x, int y, bool state)
{
    if ((x < 0) || (y < 0) || (x >= WIDTH) || (y >= HEIGHT))
        return;

    uint8_t matrix_column = 0;
    while (x >= matrix_width_)
    {
        x -= matrix_width_;
        matrix_column++;
    }
    if (is_stacked_)
    {
        matrix_column <<= 1;            
        if (y < matrix_height_)
        {
            matrix_column++;
            x = matrix_width_ - x - 1;
            y = matrix_height_ - y - 1;
        }
        else {
            y = y - matrix_height_;
        }
    }

    sendData(x, y, matrix_column, state);
    delayMicroseconds(pulseLengthMicroseconds);
    sendZeros();
    delayMicroseconds(pauseLengthMicroseconds);
}

void FlipDot34NanoGFX::sendZeros()
{
    digitalWrite(latchPin, LOW);
    for (int i = 0; i < 24; i++)
    {
        bitClear(PORTD, clockPin); //clockOff();
        bitClear(PORTD, dataPin);  //dataOff();
        bitSet(PORTD, clockPin);   //clockOn();
    }
    digitalWrite(latchPin, HIGH);
}

uint8_t addressMap[28] = {
// 2800 address lines when acessed over 74hc595 have bit order: (low bit) A0, A1, A2, B0, B1 (high bit)
    1, 2, 3, 4, 5, 6, 7,
    9, 10, 11, 12, 13, 14, 15,
    17, 18, 19, 20, 21, 22, 23,
    25, 26, 27, 28, 29, 30, 31
};

#ifdef BOARD_SMD

#define BIT_X_A0 21
#define BIT_X_A1 19
#define BIT_X_A2 17
#define BIT_X_B0 14
#define BIT_X_B1 12

#define BIT_Y_A0 10
#define BIT_Y_A1 8
#define BIT_Y_A2 5
#define BIT_Y_B0 3
#define BIT_Y_B1 1

#define BIT_X_DATA 22
#define BIT_Y_SET 20
#define BIT_Y_RESET 18

#define BIT_ENABLE_X1 16
#define BIT_ENABLE_X2 13
#define BIT_ENABLE_X3 11
#define BIT_ENABLE_X4 9
#define BIT_ENABLE_X5 6
#define BIT_ENABLE_X6 4
#define BIT_ENABLE_X7 2
#define BIT_ENABLE_X8 0

#elif BOARD_PTH

#define BIT_X_A0 3
#define BIT_X_A1 4
#define BIT_X_A2 5
#define BIT_X_B0 6
#define BIT_X_B1 8

#define BIT_Y_A0 9
#define BIT_Y_A1 10
#define BIT_Y_A2 11
#define BIT_Y_B0 12
#define BIT_Y_B1 13

#define BIT_X_DATA 2
#define BIT_Y_SET 1
#define BIT_Y_RESET 0

#define BIT_ENABLE_X1 23
#define BIT_ENABLE_X2 22
#define BIT_ENABLE_X3 21
#define BIT_ENABLE_X4 20
#define BIT_ENABLE_X5 19
#define BIT_ENABLE_X6 18
#define BIT_ENABLE_X7 17
#define BIT_ENABLE_X8 16

#endif // BOARD_SMD / BOARD_PTH

void FlipDot34NanoGFX::sendData(uint8_t x, uint8_t y, uint8_t column, bool state)
{
    x = addressMap[x];
    y = addressMap[y];

    uint32_t data = 0;

    bitWrite(data, BIT_X_A0, bitRead(x, 0));
    bitWrite(data, BIT_X_A1, bitRead(x, 1));
    bitWrite(data, BIT_X_A2, bitRead(x, 2));
    bitWrite(data, BIT_X_B0, bitRead(x, 3));
    bitWrite(data, BIT_X_B1, bitRead(x, 4));

    bitWrite(data, BIT_Y_A0, bitRead(y, 0));
    bitWrite(data, BIT_Y_A1, bitRead(y, 1));
    bitWrite(data, BIT_Y_A2, bitRead(y, 2));
    bitWrite(data, BIT_Y_B0, bitRead(y, 3));
    bitWrite(data, BIT_Y_B1, bitRead(y, 4));

    if (state) {
        bitWrite(data, BIT_X_DATA, 1);
        bitWrite(data, BIT_Y_SET, 1);
    } else {
        bitWrite(data, BIT_Y_RESET, 1);
    }

    switch (column) {
        case 0:
            bitWrite(data, BIT_ENABLE_X1, 1);    
            break;
        case 1:
            bitWrite(data, BIT_ENABLE_X2, 1);    
            break;
        case 2:
            bitWrite(data, BIT_ENABLE_X3, 1);    
            break;
        case 3:
            bitWrite(data, BIT_ENABLE_X4, 1);    
            break;
        case 4:
            bitWrite(data, BIT_ENABLE_X5, 1);    
            break;            
        case 5:
            bitWrite(data, BIT_ENABLE_X6, 1);    
            break;            
        case 6:
            bitWrite(data, BIT_ENABLE_X7, 1);    
            break;            
        case 7:
            bitWrite(data, BIT_ENABLE_X8, 1);    
            break;            
    }

    sendDataRaw(data);
}

void FlipDot34NanoGFX::sendDataRaw(uint32_t data)
{
    digitalWrite(latchPin, LOW);

    // upload address signals to shift registers U2 and U3
    for (int i = 0; i < 24; i++)
    {
        bitClear(PORTD, clockPin); //clockOff();
        //--- Turn data on or off based on value of bit
        if (data & 0x01)
        {
            bitSet(PORTD, dataPin); // dataOn();
        }
        else
        {
            bitClear(PORTD, dataPin); //dataOff();
        }
        data >>= 1;
        //register shifts bits on upstroke of clock pin
        bitSet(PORTD, clockPin); //clockOn();
    }
    digitalWrite(latchPin, HIGH);
}

void FlipDot34NanoGFX::clearScreen()
{
    for (int16_t y = 0; y < HEIGHT; y++)
    {
        for (int16_t x = 0; x < WIDTH; x++)
        {
            setDot(x, y, false);
        }
    }
}

// overwrites Adafruit
void FlipDot34NanoGFX::endWrite(void)
{

#ifdef BOARD_SMD
    Serial.println("Board type: SMD");
#else
    Serial.println("Board type: not SMD");
#endif // DEBUG

    uint8_t *buf_ptr = frameBuffer;
    uint8_t *canvas_ptr = getBuffer();

    for (int16_t y = 0; y < HEIGHT; y++) 
    {
        uint8_t mask = 0x80;
        uint8_t buf_val = *buf_ptr;
        uint8_t canvas_val = *canvas_ptr;
        for (int16_t x = 0; x < WIDTH; x++)
        {
            if ((buf_val ^ canvas_val) & mask)
            {
                setDot(x, y, canvas_val & mask);
            }
            mask >>= 1;
            if (!mask)
            {
                mask = 0x80;
                buf_ptr++;
                canvas_ptr++;
                buf_val = *buf_ptr;
                canvas_val = *canvas_ptr;
            }
        }
        if (WIDTH & 0x07)
        {
            buf_ptr++;
            canvas_ptr++;
        }
    }
    //store previous frame Buffer
    memcpy(frameBuffer, getBuffer(), frameBufferSize);
}