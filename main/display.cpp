#include <SPI.h>
#include "Adafruit_EPD.h"
#include "display.h"
#include "font.h"
#include "freertos/semphr.h"

SemaphoreHandle_t display_mux; // anything that calls the display object needs locked.
SemaphoreHandle_t display_draw_mux; // anything that calls the display object needs locked.

Adafruit_EPD *display = 0;

#define EPD_CS 26
#define EPD_DC 25
#define EPD_RESET 17
#define EPD_BUSY 27
#define EPD_MOSI 23

void setupDisplay()
{
    display_mux = xSemaphoreCreateMutex();
    display_draw_mux = xSemaphoreCreateMutex();
    display = new Adafruit_EPD(600, 448, EPD_DC, EPD_RESET, EPD_CS, EPD_BUSY);

    display->begin();
    display->clearBuffer();
}

uint8_t read8(const unsigned char *buffer, uint32_t &buffPos)
{
    return buffer[buffPos++];
}

uint16_t read16(const unsigned char *buffer, uint32_t &buffPos)
{
    uint16_t result;
    uint8_t *resultBytes = (uint8_t *)&result;
    resultBytes[0] = read8(buffer, buffPos); // LSB
    resultBytes[1] = read8(buffer, buffPos); // MSB
    return result;
}

uint32_t read32(const unsigned char *buffer, uint32_t &buffPos)
{
    uint16_t result;
    uint8_t *resultBytes = (uint8_t *)&result;
    resultBytes[0] = read8(buffer, buffPos); // LSB
    resultBytes[1] = read8(buffer, buffPos);
    resultBytes[2] = read8(buffer, buffPos);
    resultBytes[3] = read8(buffer, buffPos); // MSB
    return result;
}

void bmpDraw(const unsigned char *bmpFile, int16_t x, int16_t y, uint8_t overrideColor)
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    int bmpWidth, bmpHeight; // W+H in pixels
    uint8_t bmpDepth;        // Bit depth (currently must be 24)
    uint32_t bmpImageoffset; // Start of image data in file
    uint32_t rowSize;        // Not always = bmpWidth; may have padding
    boolean goodBmp = false; // Set to true on valid header parse
    boolean flip = true;     // BMP is stored bottom-to-top
    int row, col, x2, y2;    // Current row/column being drawn, outer bounds of bmp
    uint8_t twoPixels, pixelcolor;
    bool firstPixel;
    float bytesPerRow;
    uint32_t buffPos = 0;

    if (read16(bmpFile, buffPos) == 0x4D42)
    {                                              // BMP signature
        read32(bmpFile, buffPos);                  // File size (Ignore)
        read32(bmpFile, buffPos);                  // Read (Ignore)
        bmpImageoffset = read32(bmpFile, buffPos); // Start of image data
        read32(bmpFile, buffPos);                  // Header size (Ignore)
        bmpWidth = read32(bmpFile, buffPos);       // Width
        bmpHeight = read32(bmpFile, buffPos);      // Height
        read16(bmpFile, buffPos);                  // Planes (Ignore)
        bmpDepth = read16(bmpFile, buffPos);       // bits per pixel

        if (bmpDepth == 4) // There's more header data after this but it's irrelevant
        {
            goodBmp = true; // Supported BMP format -- proceed!
                            // round rowSize up to the nearest multiple of 4 for padding
            bytesPerRow = (float)bmpWidth * ((float)bmpDepth / 8.0);
            rowSize = (int)((bytesPerRow / 4)) + ((fmod(bytesPerRow, 4)) > 0);
            rowSize *= 4;
            flip = bmpHeight >= 0;      // Flip the image if it's got a negative height
            bmpHeight = abs(bmpHeight); // Get abs(bmpHeight) in case it's negative (flipped)
            x2 = x + bmpWidth - 1;      // Right X bound for the bitmap
            y2 = y + bmpHeight - 1;     // Bottom Y bound for the bitmap
            for (row = 0; row < bmpHeight; row++)
            {
                if (flip)
                {
                    buffPos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
                }
                else
                {
                    buffPos = bmpImageoffset + row * rowSize;
                }

                firstPixel = true; // 4bpp, Two pixels per byte, read one byte every two iterations.
                for (col = 0; col < bmpWidth; col++)
                {
                    if (firstPixel) // Reading the first 4 bits of a byte for the first 4bpp pixel
                    {
                        twoPixels = read8(bmpFile, buffPos);
                        pixelcolor = twoPixels >> 4;
                    }
                    else
                    {
                        pixelcolor = twoPixels & 0xf; // Reading the second 4 bites of a byte for the second 4bpp pixel
                    }

                    firstPixel = !firstPixel;
                    if (pixelcolor > 6)
                    {
                        pixelcolor = ACEP_COLOR_WHITE;
                    }
                    if (overrideColor < 7 && pixelcolor != 1) // Draw every non-white pixel as overrideColor (Used for drawing strings)
                    {
                        pixelcolor = overrideColor;
                    }
                    display->drawPixel(x + col, y + row, pixelcolor);
                }
            }
        }
        else
            Serial.println("Unsupported BMP bitdepth");
    }
    else
        Serial.println("Invalid BMP Magic");
    xSemaphoreGive(display_mux);
}

void sendFrameBufferAsBMP(WiFiClient webClient)
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    // Begin BMP header
    webClient.printf("%c%c", 0x42, 0x4D);                 // Magic (BM)
    webClient.printf("%c%c%c%c", 0x00, 0x00, 0x00, 0x00); // File Size (Ignore)
    webClient.printf("%c%c%c%c", 0x00, 0x00, 0x00, 0x00); // Read (Ignore)
    webClient.printf("%c%c%c%c", 0x52, 0x00, 0x00, 0x00); // File Start Offset (Starts at 0x52)
    // Begin DIB header
    webClient.printf("%c%c%c%c", 0x28, 0x00, 0x00, 0x00); // Header Size (40)
    webClient.printf("%c%c%c%c", 0x58, 0x02, 0x00, 0x00); // Width (600)
    webClient.printf("%c%c%c%c", 0x40, 0xFE, 0xFF, 0xFF); // Height (448)
    webClient.printf("%c%c", 0x01, 0x00);                 // Planes (Ignore, always 1)
    webClient.printf("%c%c", 0x04, 0x00);                 // BPP (4)
    webClient.printf("%c%c%c%c", 0x00, 0x00, 0x00, 0x00); // Compression Type (0=none)
    webClient.printf("%c%c%c%c", 0x00, 0x08, 0x00, 0x00); // Image size (Unused)
    webClient.printf("%c%c%c%c", 0x23, 0x2E, 0x00, 0x00); // Horiz resolution
    webClient.printf("%c%c%c%c", 0x23, 0x2E, 0x00, 0x00); // Vert resolution
    webClient.printf("%c%c%c%c", 0x07, 0x00, 0x00, 0x00); // Number of colors (7)
    webClient.printf("%c%c%c%c", 0x07, 0x00, 0x00, 0x00); // Number of important colors (0=all, 7 also is all)

    // Palette data              Blue, Green, Red, Alpha
    webClient.printf("%c%c%c%c", 0x00, 0x00, 0x00, 0x00); // Black
    webClient.printf("%c%c%c%c", 0xFF, 0xFF, 0xFF, 0x00); // White
    webClient.printf("%c%c%c%c", 0xAF, 0xFF, 0x00, 0x00); // Green
    webClient.printf("%c%c%c%c", 0xFF, 0x00, 0x00, 0x00); // Blue
    webClient.printf("%c%c%c%c", 0x00, 0x00, 0xFF, 0x00); // Red
    webClient.printf("%c%c%c%c", 0x00, 0xFF, 0xFF, 0x00); // Yellow
    webClient.printf("%c%c%c%c", 0x00, 0x80, 0xFF, 0x00); // Orange

    webClient.write(display->buffer1, display->buffer1_size);
    webClient.write(display->buffer2, display->buffer2_size);
    webClient.write(display->buffer3, display->buffer3_size);
    xSemaphoreGive(display_mux);
}

void drawHorizLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    for (uint16_t X = X1; X != X2; X1 < X2 ? X++ : X--)
        display->drawPixel(X, Y, color);
    xSemaphoreGive(display_mux);
}

void drawVertLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    for (uint16_t Y = Y1; Y != Y2; Y1 < Y2 ? Y++ : Y--)
        display->drawPixel(X, Y, color);
    xSemaphoreGive(display_mux);
}

void drawRect(uint16_t X, uint16_t Width, uint16_t Y, uint16_t Height, uint16_t color)
{
    drawHorizLine(X, X + Width, Y, color);
    drawHorizLine(X, X + Width, Y + Height, color);
    drawVertLine(X, Y, Y + Height, color);
    drawVertLine(X + Width, Y, Y + Height + 1, color);
}

void drawIcon(const unsigned char *icon, uint16_t X, uint16_t Y)
{
    bmpDraw(icon, X, Y);
}

void drawString(uint16_t X, uint16_t Y, const char *string, uint8_t color)
{
    for (int i = 0; string[i]; i++)
    {
        const unsigned char *theChar = charmap[string[i]];
        if (theChar == 0)
        {
            continue; // Leave the spot blank
        }
        bmpDraw(theChar, X + (i * 9), Y, color);
    }
}

void setPixel(uint16_t X, uint16_t Y, uint8_t color)
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    display->drawPixel(X, Y, color);
    xSemaphoreGive(display_mux);
}

void updateDisplay()
{
    while (!xSemaphoreTake(display_draw_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    display->display();
    xSemaphoreGive(display_draw_mux);
}

void clearDisplay()
{
    while (!xSemaphoreTake(display_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    display->clearBuffer();
    xSemaphoreGive(display_mux);
}
