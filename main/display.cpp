#include <SPI.h>
#include "Adafruit_EPD.h"
#include "display.h"
#include "font.h"

Adafruit_EPD *display = 0;

#define EPD_CS 26
#define EPD_DC 25
#define EPD_RESET 17
#define EPD_BUSY 27
#define EPD_MOSI 23

void display_setup()
{
  display = new Adafruit_EPD(600, 448, EPD_DC, EPD_RESET, EPD_CS, EPD_BUSY);

  display->begin();
  display->clearBuffer();
}

unsigned long buffPos = 0;

uint8_t read8(const unsigned char *bmpFile)
{
  return bmpFile[buffPos++];
}

uint16_t read16(const unsigned char *bmpFile)
{
  uint16_t result;
  ((uint8_t *)&result)[0] = read8(bmpFile); // LSB
  ((uint8_t *)&result)[1] = read8(bmpFile); // MSB
  return result;
}

uint32_t read32(const unsigned char *bmpFile)
{
  uint32_t result;
  ((uint8_t *)&result)[0] = read8(bmpFile); // LSB
  ((uint8_t *)&result)[1] = read8(bmpFile);
  ((uint8_t *)&result)[2] = read8(bmpFile);
  ((uint8_t *)&result)[3] = read8(bmpFile); // MSB
  return result;
}

void drawHorizLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color)
{
  for (uint16_t X = X1; X != X2; X1 < X2 ? X++ : X--)
    display->drawPixel(X, Y, color);
}

void drawVertLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color)
{
  for (uint16_t Y = Y1; Y != Y2; Y1 < Y2 ? Y++ : Y--)
    display->drawPixel(X, Y, color);
}

void drawRect(uint16_t X, uint16_t Width, uint16_t Y, uint16_t Height, uint16_t color)
{
  drawHorizLine(X, X + Width, Y, color);
  drawHorizLine(X, X + Width, Y + Height, color);
  drawVertLine(X, Y, Y + Height, color);
  drawVertLine(X + Width, Y, Y + Height + 1, color);
}

void bmpDraw(const unsigned char *bmpFile, uint32_t bmpFileLength, int16_t x, int16_t y, uint8_t overrideColor)
{
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
  buffPos = 0;

  if (read16(bmpFile) == 0x4D42)
  {                                   // BMP signature
    read32(bmpFile);                  // File size (Ignore)
    read32(bmpFile);                  // Read (Ignore)
    bmpImageoffset = read32(bmpFile); // Start of image data
    read32(bmpFile);                  // Header size (Ignore)
    bmpWidth = read32(bmpFile);       // Width
    bmpHeight = read32(bmpFile);      // Height
    read16(bmpFile);                  // Planes (Ignore)
    bmpDepth = read16(bmpFile);       // bits per pixel

    if (bmpDepth == 4) // There's more header data after this but it's irrelevant
    {
      goodBmp = true; // Supported BMP format -- proceed!
                      // round rowSize up to the nearest multiple of 4 for padding
      bytesPerRow = (float)bmpWidth * ((float)bmpDepth / 8.0);
      rowSize = (int)((bytesPerRow / 4)) + ((fmod(bytesPerRow, 4)) > 0);
      rowSize *= 4;
      flip = bmpHeight >= 0;      // Flip the image if it's got a negative height
      bmpHeight = abs(bmpHeight); // Get abs(bmpHeight) it in case it's negative (flipped)
      x2 = x + bmpWidth - 1;      // Right X bound for the bitmap
      y2 = y + bmpHeight - 1;     // Bottom Y bound for the bitmap
      Serial.printf("Drawing bmp x,y %i,%i width,height %i,%i\n",x,y,x2,y2);
      for (row = 0; row < bmpHeight; row++)
      {
        if (flip)
          buffPos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
        else
          buffPos = bmpImageoffset + row * rowSize;

        firstPixel = true; // 4bpp, Two pixels per byte, read one byte every two iterations.
        for (col = 0; col < bmpWidth; col++)
        {
          if (firstPixel) // Reading the first 4 bits of a byte for the first 4bpp pixel
          {
            twoPixels = read8(bmpFile);
            pixelcolor = twoPixels >> 4;
          }
          else
            pixelcolor = twoPixels & 0xf; // Reading the second 4 bites of a byte for the second 4bpp pixel

          firstPixel = !firstPixel;
          if (pixelcolor > 6)
            pixelcolor = ACEP_COLOR_WHITE;
          if (overrideColor < 7 && pixelcolor != 1) // Draw every non-white pixel as overrideColor (Used for drawing strings)
            pixelcolor = overrideColor;
          display->drawPixel(x + col, y + row, pixelcolor);
        }
      }
    }
    else
      Serial.println("Unsupported BMP bitdepth");
  }
  else
    Serial.println("Invalid BMP Magic");
}

void drawIcon(const unsigned char *icon, uint16_t X, uint16_t Y)
{
  bmpDraw(icon, 0, X, Y);
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
    bmpDraw(theChar, 0, X + (i * 9), Y, color);
  }
}

void setPixel(uint16_t X, uint16_t Y, uint8_t color)
{
  display->drawPixel(X, Y, color);
}

void updateDisplay()
{
  display->display();
}

void clearDisplay()
{
  display->clearBuffer();
}
