#include "menu.h"
#include <string>
#include <ArduinoOTA.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "graphics.h"
button *displaybuttons[MENU_ROWS * MENU_COLUMNS];

button *topbuttons[TOPBUTTONCOUNT];

RTC_DATA_ATTR uint8_t curMenuIndex = 0;

extern struct menu menus[];
extern uint32_t numMenus;

extern struct button buttons[];
extern uint32_t numButtons;

Adafruit_TSC2007 touch;

struct hotspot hotspots[MAX_HOTSPOTS];
uint8_t numHotspots = 0;

int fakeClickX = -1;
int fakeClickY = -1;

uint16_t x = 0, y = 0, z1 = 0, z2 = 0;

void dumpConfig()
{

    Serial.printf("\n\nMenus: %i\n", numMenus);
    for (int i = 0; i < numMenus; i++)
    {
        Serial.printf("Menu\n Name: %s\n Icon: %s\n Label: %s\n", menus[i]._name, menus[i].icon, menus[i].label);
        Serial.printf("  Buttons:\n");
        for (int j = 0; j < menus[i].numButtons; j++)
        {
            // Serial.printf("    %s\n", menus[i].buttons.entries[j].entry);
        }
    }

    Serial.printf("\n\nButtons: %i\n", numButtons);
    for (int i = 0; i < numButtons; i++)
    {
        Serial.printf("Button\n Name: %s\n Icon: %s\n Label: %s\n", buttons[i]._name, buttons[i].icon, buttons[i].label);
        Serial.printf("  Actions: %i\n", buttons[i].numActions);
        for (int j = 0; j < buttons[i].numActions; j++)
        {
            switch (buttons[i].actions[j].type)
            {
            case ACTION_HASERVICE:
                Serial.printf("    Call Home Assistant:\n");
                Serial.printf("      Service: %s\n", buttons[i].actions[j].data.haservice.service);
                Serial.printf("      target_json: %s\n", buttons[i].actions[j].data.haservice.target_json);
                Serial.printf("      data_json: %s\n", buttons[i].actions[j].data.haservice.data_json);
                break;
            case ACTION_URL:
                break;
            case ACTION_NATIVE:
                break;
            case ACTION_JUMP:
                break;
            }
        }
    }
}

button *getButtonFromName(const char *name)
{
    for (int i = 0; i < numButtons; i++)
    {
        if (strcmp(name, buttons[i]._name) == 0)
            return &buttons[i];
    }
    Serial.printf("Could not find button %s\n", name);
    return 0;
}
bool otaSetup = false;

void handleOTA()
{
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
        return;
    if (otaSetup)
    {
        ArduinoOTA.handle();
    }
    else
    {
        otaSetup = true;
        ArduinoOTA
            .onStart([]()
                     {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type); })
            .onEnd([]()
                   { Serial.println("\nEnd"); })
            .onProgress([](unsigned int progress, unsigned int total)
                        { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
            .onError([](ota_error_t error)
                     {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR) Serial.println("End Failed"); });
        ArduinoOTA.setPort(3232);
        ArduinoOTA.begin();
    }
}

void setupTouch()
{
    int tryCount = 10;
    while (tryCount-- & !touch.begin())
    {
        Serial.printf("Trying again to init touchscreen");
        delay(10);
    }
    if (!tryCount)
    {
        Serial.printf("Failed to init touchscreen!");
    }
}

void setupMenu()
{
    setupTouch();
    handleOTA();
    touch.read_touch(&y, &x, &z1, &z2); // Swap y,x because screen is rotated, read as early in the startup process as possible
    updateMenu(false);
}

void updateBody(bool renderMenu)
{
    for (int row = 0; row < MENU_ROWS; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            uint8_t i = (row * MENU_COLUMNS) + col;
            if (menus[curMenuIndex].numButtons > i)
            {
                displaybuttons[i] = getButtonFromName(menus[curMenuIndex].buttons[i]);
                if (displaybuttons[i])
                {
                    hotspots[numHotspots].x = 2 + (col * 100);
                    hotspots[numHotspots].y = 38 + (row * 100);
                    hotspots[numHotspots].width = 94;
                    hotspots[numHotspots].height = 94;
                    hotspots[numHotspots].numActions = displaybuttons[i]->numActions;
                    hotspots[numHotspots].actions = displaybuttons[i]->actions;
                    numHotspots++;
                }
            }
            else
            {
                displaybuttons[i] = 0;
            }
        }
    }
    if (!renderMenu)
        return;
    for (int row = 0; row < MENU_ROWS; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            uint8_t index = (row * MENU_COLUMNS) + col;
            if (displaybuttons[index])
            {
                drawIcon(displaybuttons[index]->iconptr, 17 + (col * 100), 48 + (row * 100));
                drawString(5 + (col * 100), 118 + (row * 100), displaybuttons[index]->label, ACEP_COLOR_BLACK);
                drawRect(2 + (col * 100), 94, 38 + (row * 100), 94, ACEP_COLOR_BLACK);
            }
        }
    }
}

void updateColorSelectWidget(bool renderMenu, uint16_t left, uint16_t top)
{
    const uint8_t widgetButtonCount = 6;
    const char *widgetButtons[widgetButtonCount] = {"red_slider", "green_slider", "blue_slider", "white_slider", "brightness_slider", "colortemp_slider"};

    for (int i = 0; i < widgetButtonCount; i++)
    {
        struct button *widgetButton = getButtonFromName(widgetButtons[i]);

        hotspots[numHotspots].x = left + 120;
        hotspots[numHotspots].y = top;
        hotspots[numHotspots].width = 240;
        hotspots[numHotspots].height = 32;
        hotspots[numHotspots].numActions = widgetButton->numActions;
        hotspots[numHotspots].actions = widgetButton->actions;
        numHotspots++;
        if (renderMenu)
        {
            drawIcon(widgetButton->iconptr, left + 120, top);
            drawString(left, top + 10, widgetButton->label, 0);
        }
        top += 34;
    }
}

void updateBodyLights(bool renderMenu)
{
    for (int row = 0; row < MENU_ROWS - 2; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            uint8_t i = (row * MENU_COLUMNS) + col;
            if (menus[curMenuIndex].numButtons > i)
            {
                displaybuttons[i] = getButtonFromName(menus[curMenuIndex].buttons[i]);
                if (displaybuttons[i])
                {
                    hotspots[numHotspots].x = 2 + (col * 100);
                    hotspots[numHotspots].y = 38 + (row * 100);
                    hotspots[numHotspots].width = 94;
                    hotspots[numHotspots].height = 94;
                    hotspots[numHotspots].numActions = displaybuttons[i]->numActions;
                    hotspots[numHotspots].actions = displaybuttons[i]->actions;
                    numHotspots++;
                }
            }
            else
            {
                displaybuttons[i] = 0;
            }
        }
    }

    uint16_t top = 6 + 38 + 94 + (1 * 100);
    uint16_t left = 6;
    updateColorSelectWidget(renderMenu, left, top);

    if (!renderMenu)
        return;
    for (int row = 0; row < MENU_ROWS - 2; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            uint8_t index = (row * MENU_COLUMNS) + col;
            if (displaybuttons[index])
            {
                drawIcon(displaybuttons[index]->iconptr, 17 + (col * 100), 48 + (row * 100));
                drawString(5 + (col * 100), 118 + (row * 100), displaybuttons[index]->label, ACEP_COLOR_BLACK);
                drawRect(2 + (col * 100), 94, 38 + (row * 100), 94, ACEP_COLOR_BLACK);
            }
        }
    }
}

void updateTopBar(bool renderMenu)
{
    int topbarindex = getMenuIndexFromName("TopBar");
    if (topbarindex != -1)
    {
        for (int i = 0; i < TOPBUTTONCOUNT; i++)
        {
            if (menus[topbarindex].numButtons > i)
            {
                topbuttons[i] = getButtonFromName(menus[topbarindex].buttons[i]);
                hotspots[numHotspots].x = 600 - 36 * (i + 1);
                hotspots[numHotspots].y = 0;
                hotspots[numHotspots].width = 32;
                hotspots[numHotspots].height = 32;
                hotspots[numHotspots].numActions = topbuttons[i]->numActions;
                hotspots[numHotspots].actions = topbuttons[i]->actions;
                numHotspots++;
            }
            else
            {
                topbuttons[i] = 0;
            }
        }
    }
    else
    {
        Serial.printf("Menu TopBar not found!\n");
    }

    if (!renderMenu)
        return;

    drawString(0, 9, menus[curMenuIndex]._name, 0);
    drawHorizLine(0, 640, 35, ACEP_COLOR_BLACK);
    for (int i = 0; i < TOPBUTTONCOUNT; i++)
    {
        if (topbuttons[i])
        {
            drawIcon(topbuttons[i]->iconptr, 600 - 36 * (i + 1), 0);
        }
    }
}

void updateMenu(bool renderMenu)
{
    memset(hotspots, 0, sizeof(hotspot) * MAX_HOTSPOTS);
    numHotspots = 0;
    if (renderMenu)
    {
        clearDisplay();
    }
    if (strcmp(menus[curMenuIndex]._name, "Lights") == 0)
    {
        updateBodyLights(renderMenu);
    }
    else
    {
        updateBody(renderMenu);
    }
    updateTopBar(renderMenu);
}

uint8_t getMenuIndexFromName(const char *name)
{
    for (int i = 0; i < numMenus; i++)
        if (strcmp(name, menus[i]._name) == 0)
            return i;
    Serial.printf("Could not find menu %s\n", name);
    return -1;
}

void runHotspot(int x, int y, int lastX, int lastY, int hotspotIndex, bool isFirstPress)
{
    int16_t relX = x - hotspots[hotspotIndex].x;
    int16_t relY = y - hotspots[hotspotIndex].y;
    int16_t relLastX = lastX - hotspots[hotspotIndex].x;
    int16_t relLastY = lastY - hotspots[hotspotIndex].y;
    taskDoActions(hotspots[hotspotIndex].actions, hotspots[hotspotIndex].numActions, relX, relY, relLastX, relLastY, isFirstPress);
}

int checkHotspot(int x, int y)
{
    for (int i = 0; i < numHotspots; i++)
    {
        if (x > hotspots[i].x && x < (hotspots[i].x + hotspots[i].width) && y > hotspots[i].y && y < (hotspots[i].y + hotspots[i].height))
        {
            return i;
        }
    }
    return -1;
}

void jumpToMenu(const char *menuName)
{
    curMenuIndex = getMenuIndexFromName(menuName);
    updateMenu(true);
    updateDisplay();
}

void normalizeTouch(uint16_t *x, uint16_t *y)
{
    uint16_t top = 3880;
    uint16_t bottom = 375;
    uint16_t left = 190;
    uint16_t right = 3915;

    *x = *x - left;                      // Remove left offset from X
    *y = (top - bottom) - (*y - bottom); // Flip Y, remove bottom offset

    *x = round((double(*x) / double(right - left)) * 600);
    *y = round((double(*y) / double(top - bottom)) * 448);
}

void setFakeTouch(int x, int y)
{
    fakeClickX = x;
    fakeClickY = y;
}

void processTouchLoop()
{
    uint32_t awakeFor = 0;
    bool firstRead = true;
    bool buttonReleased = true;
    uint32_t buttonHeldAt = 0;
    uint32_t actionLastRan = 0;
    uint32_t lastX = 0;
    uint32_t lastY = 0;
    int hotspotIndex = -1;
    int pressedHotspotIndex = -1; // Track which hotspot is pressed when a drag action is occurring
    while (awakeFor < STAY_AWAKE_MS)
    {
        handleOTA();
        if (!firstRead)
        {
            if (fakeClickX != -1 && fakeClickY != -1)
            {
                x = fakeClickX;
                y = fakeClickY;
                z1 = 2048;
                z2 = 4096;
            }
            else
            {
                //touch.read_touch(&y, &x, &z1, &z2); // Swap y,x because screen is rotated.

                //TODO: Undo when touchscreen isn't broken
                x = 0;
                y = 0;
                z1 = 0;
                z2 = 4096;
            }
            
        }
        else
        {
            firstRead = false;
        }
        if (x == 0 || y == 0 || z1 < 30) // Filter out low pressure touches and bad reads, z1 is touch pressure.
        {
            if (!buttonReleased)
            Serial.printf("Release\n");
            buttonReleased = true;
            pressedHotspotIndex = -1;
            awakeFor += 10;
            delay(10);
            continue;
        }
        if (fakeClickX == -1 || fakeClickY == -1)
            normalizeTouch(&x, &y);
        if ((buttonReleased || millis() - buttonHeldAt > 450) && millis() - actionLastRan > 100) // Wait 450ms between first press and repeating, then repeat every 100ms
        {
            hotspotIndex = checkHotspot(x, y);
            Serial.printf("hotspot index: %i\n",hotspotIndex);
            if (hotspotIndex != -1 || pressedHotspotIndex != -1)
            {
                awakeFor = 0;

                if (buttonReleased)
                {
                    Serial.printf("Button Press");
                    buttonHeldAt = millis();
                    pressedHotspotIndex = hotspotIndex;
                }
                actionLastRan = millis();
                Serial.printf("hotspotIndex: %i pressedHotspotIndex: %i\n",hotspotIndex,pressedHotspotIndex);
                runHotspot(x, y, lastX, lastY, pressedHotspotIndex, buttonReleased);
                buttonReleased = false;
                lastX = x;
                lastY = y;
            }
        }
        awakeFor += 10;
        delay(10);
    }
}