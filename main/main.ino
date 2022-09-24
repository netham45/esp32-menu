



#include "Adafruit_TSC2007.h"
#include <Wire.h>
#include <WiFi.h>
#include "display.h"
#include "menu.h"
#include "Esp.h"
#include "secrets.h"

void setupWifi()
{
    IPAddress local_IP(192, 168, 5, 136);
    IPAddress gateway(192, 168, 5, 3);
    IPAddress dns(192, 168, 5, 3);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_STA);
    WiFi.config(local_IP, dns, gateway, subnet);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(22, 21);
    setupMenu(); // Call as early as possible to get initial touchscreen reading after wakeup
    setupWifi();
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
    setupDisplay();
    processTouchLoop();
    esp_deep_sleep_start();
}

void loop()
{ // Shouldn't ever get here
    esp_deep_sleep_start();
}
