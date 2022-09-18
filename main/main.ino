

#include "Adafruit_TSC2007.h"
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "structs.h"
#include "display.h"
#include "menu.h"
#include "actions.h"
#include "Esp.h"
#include "secrets.h"

Adafruit_TSC2007 touch;

void normalizeTouch(uint16_t *x, uint16_t *y)
{
  uint16_t top = 3830;
  uint16_t bottom = 375;
  uint16_t left = 190;
  uint16_t right = 3915;

  *x = *x - left;                      // Remove left offset from X
  *y = (top - bottom) - (*y - bottom); // Flip Y, remove bottom offset

  *x = round((double(*x) / double(right - left)) * 600);
  *y = round((double(*y) / double(top - bottom)) * 448);
}

#define STAY_AWAKE_MS 30000
void setup()
{
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
  Serial.begin(115200);  Wire.begin(22, 21);
  int tryCount = 10;
  while (tryCount-- & !touch.begin())
  {
    Serial.println("Couldn't find touch controller");
    delay(10);
  }

  IPAddress local_IP(192, 168, 5, 136);
  IPAddress gateway(192, 168, 5, 3);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_STA);
  WiFi.config(local_IP, gateway, subnet);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  uint16_t x, y, z1, z2;
  touch.read_touch(&y, &x, &z1, &z2); // Swap y,x because screen is rotated.
  uint32_t awakeFor = 0;
  display_setup();
  drawMenu();
  Serial.printf("Free Heap: %i\n", ESP.getFreeHeap());
  bool firstRead = true;
  bool buttonReleased = true;
  uint32_t buttonHeldAt = 0;
  uint32_t buttonLastPressed = 0;
  while (awakeFor < STAY_AWAKE_MS)
  {
    if (!firstRead)
      touch.read_touch(&y, &x, &z1, &z2); // Swap y,x because screen is rotated.
    else
      firstRead = false;
    if (x == 0 || y == 0)
    {
      buttonReleased = true;
      awakeFor += 10;
      delay(10);
      continue;
    }
    normalizeTouch(&x, &y);
    bool hotspotFound = false;
    for (int row = 0; row < MENU_ROWS; row++)
    {
      for (int col = 0; col < MENU_COLUMNS; col++)
      {
        uint16_t top = 38 + (row * 100);
        uint16_t bottom = 94 + 38 + (row * 100);
        uint16_t left = 2 + (col * 100);
        uint16_t right = 96 + (col * 100);
        if (x > left && x < right && y > top && y < bottom)
        {
          hotspotFound = true;
          awakeFor = 0;
          uint8_t index = (row * MENU_COLUMNS) + col;
          if (buttonReleased)
          {
            buttonLastPressed = buttonHeldAt = millis();
            doButtonActionsMainIndex(index);
          }
          else if ((millis() - buttonHeldAt > 250) && (millis() - buttonLastPressed > 100))
          {
            buttonLastPressed = millis();
            doButtonActionsMainIndex(index);
          }
          buttonReleased = false;
        }
      }
    }

    for (int i = TOPBUTTONCOUNT; i > 0; i--)
    {
      uint16_t top = 0;
      uint16_t bottom = 34;
      uint16_t left = 600 - (36 * i);
      uint16_t right = 600 - (36 * i) + 32;
      if (x > left && x < right && y > top && y < bottom)
      {
        hotspotFound = true;
        awakeFor = 0;
        if (buttonReleased)
        {
          buttonLastPressed = buttonHeldAt = millis();
          doButtonActionsTopBarIndex(i-1);
        }
        else if ((millis() - buttonHeldAt > 250) && (millis() - buttonLastPressed > 100))
        {
          buttonLastPressed = millis();
          doButtonActionsTopBarIndex(i-1);
        }
        buttonReleased = false;
      }
    }

    if (!hotspotFound)
    {
      buttonReleased = true;
    }
    awakeFor += 10;
    delay(10);
  }
  esp_deep_sleep_start();
}
