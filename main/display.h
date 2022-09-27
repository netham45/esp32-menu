#ifndef display_h
#define display_h
#include "Adafruit_EPD.h"
#include "graphics.h"
#include <WiFi.h>

void setupDisplay();
void bmpDraw(const unsigned char *bmpFile, int16_t x, int16_t y, uint8_t overrideColor = 255);
void sendFrameBufferAsBMP(WiFiClient webClient);
void drawHorizLine(uint16_t X1, uint16_t X2, uint16_t Y, uint16_t color);

void drawVertLine(uint16_t X, uint16_t Y1, uint16_t Y2, uint16_t color);

void drawRect(uint16_t X, uint16_t Width, uint16_t Y, uint16_t Height, uint16_t color);

void updateDisplay();
void clearDisplay();

void drawString(uint16_t X, uint16_t Y, const char *string, uint8_t color);

void drawIcon(const unsigned char *icon, uint16_t X, uint16_t Y);

uint8_t read8(const unsigned char *buffer, uint32_t &buffPos);
uint16_t read16(const unsigned char *buffer, uint32_t &buffPos);
uint32_t read32(const unsigned char *buffer, uint32_t &buffPos);

#endif