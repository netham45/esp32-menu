#include <arduino.h>
#include "menu.h"

button *displaybuttons[MENU_ROWS * MENU_COLUMNS];

RTC_DATA_ATTR uint8_t curMenuIndex = 11;

extern struct menu menus[];
extern uint32_t numMenus;

extern struct button buttons[];
extern uint32_t numButtons;

int16_t light_red = 255, light_green = 255, light_blue = 255, light_white = 255, light_brightness = 255;
char lightName[64] = {0};

void printShit()
{

    Serial.printf("\n\nMenus: %i\n", numMenus);
    for (int i = 0; i < numMenus; i++)
    {
        Serial.printf("Menu\n Name: %s\n Icon: %s\n Label: %s\n", menus[i]._name, menus[i].icon, menus[i].label);
        Serial.printf("  Buttons:\n");
        for (int j = 0; j < menus[i].buttons.length; j++)
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
    return 0;
}

void drawMenu()
{
    uint8_t index = 0;
    for (int row = 0; row < MENU_ROWS; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            index = (row * MENU_COLUMNS) + col;
            if (menus[curMenuIndex].buttons.length > index)
                displaybuttons[index] = getButtonFromName(menus[curMenuIndex].buttons.entries[index]);
            else
                displaybuttons[index] = 0;
        }
    }

    clearDisplay();
    drawHorizLine(0, 640, 35, ACEP_COLOR_BLACK);

    drawString(0, 9, menus[curMenuIndex]._name, 0);
    for (int row = 0; row < MENU_ROWS; row++)
    {
        for (int col = 0; col < MENU_COLUMNS; col++)
        {
            index = (row * MENU_COLUMNS) + col;
            if (displaybuttons[index])
            {
                Serial.printf("placeholder: %p, &placeholder: %p\n",placeholder_bmp,&placeholder_bmp);
                drawIcon(&placeholder_bmp, 17 + (col * 100), 48 + (row * 100));
                drawString(5 + (col * 100), 118 + (row * 100), displaybuttons[index]->label, ACEP_COLOR_BLACK);
                drawRect(2 + (col * 100), 94, 38 + (row * 100), 94, ACEP_COLOR_BLACK);
            }
        }
    }
}

uint8_t getMenuIndexFromName(const char *name)
{
    for (int i = 0; i < numMenus; i++)
        if (strcmp(name, menus[i]._name) == 0)
            return i;
    Serial.printf("Could not find menu %s\n", name);
    return -1;
}

void doButtonActions(uint8_t buttonIndex)
{
    Serial.printf("Running action %i\n", buttonIndex);
    if (!displaybuttons[buttonIndex])
    {
        Serial.printf("Trying to run action for unpopulated index %i\n", buttonIndex);
        return;
    }
    for (int i = 0; i < displaybuttons[buttonIndex]->numActions; i++)
    {
        switch (displaybuttons[buttonIndex]->actions[i].type)
        {
        case ACTION_HASERVICE:
            Serial.printf("Running HAService\n");
            char service[64];
            char target_json[64];
            char data_json[64];
            strcpy(service, displaybuttons[buttonIndex]->actions[i].data.haservice.service);
            strcpy(target_json, displaybuttons[buttonIndex]->actions[i].data.haservice.target_json);
            strcpy(data_json, displaybuttons[buttonIndex]->actions[i].data.haservice.data_json);
            harequest(service,
                      target_json,
                      data_json);
            break;
        case ACTION_URL:
            Serial.printf("Running URL\n");
            // httprequest(displaybuttons[buttonIndex]->actions[i].data.url.url);
            break;
        case ACTION_NATIVE:
            Serial.printf("Running Native Command %s\n", displaybuttons[buttonIndex]->actions[i].data.native.command);
            if (strcmp("redraw", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                updateDisplay();
            }
            else if (strcmp("light_select", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                strcpy(lightName, displaybuttons[buttonIndex]->actions[i].data.native.data);
            }
            // void ha_light_set_state(const char* _lightName, bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
            else if (strcmp("light_off", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                ha_light_set_state(lightName, false, 0, 0, 0, 0, 0);
            }
            else if (strcmp("light_on", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_green_up", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_green += 10;
                if (light_green > 255)
                    light_green = 255;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_green_down", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_green -= 10;
                if (light_green < 0)
                    light_green = 0;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_red_up", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_red += 10;
                if (light_red > 255)
                    light_red = 255;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_red_down", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_red -= 10;
                if (light_red < 0)
                    light_red = 0;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_blue_up", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_blue += 10;
                if (light_blue > 255)
                    light_blue = 255;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_blue_down", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_blue -= 10;
                if (light_blue < 0)
                    light_blue = 0;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_white_up", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_white += 10;
                if (light_white > 255)
                    light_white = 255;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_white_down", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_white -= 10;
                if (light_white < 0)
                    light_white = 0;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_brightness_up", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_brightness += 10;
                if (light_brightness > 255)
                    light_brightness = 255;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else if (strcmp("light_brightness_down", displaybuttons[buttonIndex]->actions[i].data.native.command) == 0)
            {
                light_brightness -= 10;
                if (light_brightness < 0)
                    light_brightness = 0;
                ha_light_set_state(lightName, true, light_red, light_green, light_blue, light_white, light_brightness);
            }
            else 
            {
                Serial.printf("Unknown Native Command: %s\n", displaybuttons[buttonIndex]->actions[i].data.native.command);
            }

            // if (strcmp("Command",...) == 0) ...
            break;
        case ACTION_JUMP:
            Serial.printf("Running Jump\n");
            curMenuIndex = getMenuIndexFromName(displaybuttons[buttonIndex]->actions[i].data.jump.destination);
            drawMenu();
            updateDisplay();
            break;
        case ACTION_UNSET:
        default:
            Serial.printf("Running nothing (unset)\n");
            break;
        }
    }
}
