#include "FlipDot34.h"
#include "FlipDot34GFX.h"
#include "Arduino.h"

#define pulseLengthMicroseconds 400
#define pauseLengthMicroseconds 200

FlipDot34GFX::FlipDot34GFX(int matrix_width, int matrix_height, int matrix_columns, StackMode stack_mode)
    : GFXcanvas1(matrix_width * matrix_columns, stack_mode == StackMode::kRow ? matrix_height : matrix_height * 2),
    FlipDot34()
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
    clearScreen();
}

void FlipDot34GFX::setDotStart(int x, int y, bool state)
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
            y = matrix_height_ - y - 1;
        }
        else
        {
            matrix_column <<= 1;
            y = y - matrix_height_;
        }
    }
    sendData(x, y, matrix_column, state);
}

void FlipDot34GFX::setDotEnd() {
    sendZeros();
}

void FlipDot34GFX::clearScreen()
{
    for (int16_t y = 0; y < HEIGHT; y++)
    {
        for (int16_t x = 0; x < WIDTH; x++)
        {
            setDotStart(x, y, false);
            delayMicroseconds(pulseLengthMicroseconds);
            setDotEnd();
            delayMicroseconds(pauseLengthMicroseconds);
        }
    }
}

// overwrites Adafruit
void FlipDot34GFX::endWrite(void)
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
                setDotStart(x, y, canvas_val & mask);
                delayMicroseconds(pulseLengthMicroseconds);
                setDotEnd();
                delayMicroseconds(pauseLengthMicroseconds);
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