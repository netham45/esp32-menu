#ifndef display_h
#define display_h
#include "Adafruit_EPD.h"

void display_setup();
void bmpDraw(const unsigned char *bmpFile, uint32_t bmpFileLength, int16_t x, int16_t y, uint8_t overrideColor = 255);
void drawHorizLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color);

void drawVertLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color);

void drawRect(uint16_t X, uint16_t Width, uint16_t Y, uint16_t Height, uint16_t color);

void updateDisplay();
void clearDisplay();

void drawString(uint16_t X, uint16_t Y, const char *string, uint8_t color);

void drawIcon(const unsigned char* icon, uint16_t X, uint16_t Y);

#endif
