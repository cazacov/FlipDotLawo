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
        if (y < matrix_height_)
        {
            matrix_column <<= 1;
            matrix_column++;
            x = matrix_width_ - x - 1;
        }
        else
        {
            matrix_column <<= 1;
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

// 2800 address lines when acessed over 74hc595 have bit order: (high bit) B1, B0, A2, A1, A0 (low bit)

uint8_t addressMap[28] = {
    16, 8, 24, 4, 20, 12, 28,
    18, 10, 26, 6, 22, 14, 30,
    17, 9, 25, 5, 21, 13, 29,
    19, 11, 27, 7, 23, 15, 31};

void FlipDot34NanoGFX::sendData(uint8_t x, uint8_t y, uint8_t column, bool state)
{

    x = addressMap[x];
    if (!is_stacked_)
    {
        y = addressMap[y];
    }
    else
    {
        if (y < matrix_height_)
        {
            y = addressMap[matrix_height_ - y - 1];
        }
        else
        {
            y = addressMap[y - matrix_height_];
        }
    }

    uint16_t addr = x;
    addr <<= 8;
    if (x & 0x01)
    {
        addr |= 0x80;
    }
    addr |= (y << 2);

    // Set X DATA
    if (state)
    {
        addr |= 0x2000;
    }
    // Set Y ENABLE
    if (state)
    {
        addr |= 0x4000;
    }
    else
    {
        addr |= 0x8000;
    }

    uint8_t enable = 0x01 << column;

    sendDataRaw(addr, enable);
}

void FlipDot34NanoGFX::sendDataRaw(uint16_t address, uint8_t enable)
{

    digitalWrite(latchPin, LOW);

    // upload address signals to shift registers U2 and U3
    for (int i = 0; i < 16; i++)
    {
        bitClear(PORTD, clockPin); //clockOff();
        //--- Turn data on or off based on value of bit
        if (address & 0x8000)
        {
            bitSet(PORTD, dataPin); // dataOn();
        }
        else
        {
            bitClear(PORTD, dataPin); //dataOff();
        }
        address <<= 1;
        //register shifts bits on upstroke of clock pin
        bitSet(PORTD, clockPin); //clockOn();
    }

    // upload enable signals to shift register U1
    for (int i = 0; i < 8; i++)
    {
        bitClear(PORTD, clockPin); //clockOff();
        //--- Turn data on or off based on value of bit
        if (enable & 0x80)
        {
            bitSet(PORTD, dataPin); // dataOn();
        }
        else
        {
            bitClear(PORTD, dataPin); //dataOff();
        }
        enable <<= 1;
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