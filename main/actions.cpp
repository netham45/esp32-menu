#include <arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "structs.h"
#include "display.h"
#include "menu.h"
#include "actions.h"
#include "secrets.h"

void harequest(const char *service, const char *target_json, const char *data_json)
{
    Serial.printf("harequest\nService: %s\n Target JSON: %s\n, Data JSON: %s\n", service, target_json, data_json);
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, "192.168.5.107", 8123);
    uint32_t maxwait = 100; // 10ms per tick
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(10);
        if (!maxwait--)
        {
            Serial.println("Failed to connect to wifi");
            return;
        }
    }
    char servicepath[64] = {0};
    sprintf(servicepath, "/api/services/%s", service);
    for (int i = 0; servicepath[i]; i++)
        if (servicepath[i] == '.')
            servicepath[i] = '/';
    Serial.printf("Service Path: %s\n", servicepath);
    uint8_t counter = 0;

    http.beginRequest();
    http.setHttpResponseTimeout(1000);
    uint8_t result = http.post(servicepath);
    if (result != 0)
    {
        Serial.printf("Request timeout\n");
        return;
    }
    http.setHttpResponseTimeout(4000);
    http.setTimeout(100);
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("authorization", HA_TOKEN);
    http.sendHeader("Content-Length", strlen(data_json));
    http.beginBody();
    http.print(data_json);
    http.endRequest();
    Serial.printf("Got response: %i\n", http.responseStatusCode());
}

void ha_light_set_state(const char *_lightName, bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
{
    char service[32];
    char data[128];
    if (turnOn)
        strcpy(service, "light.turn_on");
    else
        strcpy(service, "light.turn_off");
    sprintf(data, "{\"entity_id\": \"%s\", \"rgbw_color\": [%i, %i, %i, %i], \"brightness\":%i} ", _lightName, red, green, blue, white, brightness);
    harequest(service, "", data);
}

void httpRequest()
{
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, "192.168.5.107", 8123);
    Serial.println("\nhttpRequest");
    uint32_t maxwait = 100; // 10ms per tick
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(10);
        if (!maxwait--)
        {
            Serial.println("Failed to connect to wifi");
            return;
        }
    }
    char postData[512] = "{\"entity_id\": \"light.livingroom_all_lights\"}";
    http.beginRequest();
    http.post("/api/services/light/toggle");
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("authorization", HA_TOKEN);
    http.sendHeader("Content-Length", strlen(postData));
    http.beginBody();
    http.print(postData);
    http.endRequest();
    Serial.printf("post status: %i\n", http.responseStatusCode());
}

void loop()
{
}
