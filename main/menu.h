#ifndef menu_h
#define menu_h
#include <arduino.h>
#include "display.h"
#include "actions.h"
#include "Adafruit_TSC2007.h"
#define MENU_ROWS 4
#define MENU_COLUMNS 6
#define TOPBUTTONCOUNT 12

#define MAX_HOTSPOTS 50
#define STAY_AWAKE_MS 30000

struct button
{
    const char *_name;
    const char *icon;
    const unsigned char *iconptr;
    const char *label;
    const int numActions;
    const struct action *actions;
};

struct menu
{
    const char *_name;
    const char *icon;
    const unsigned char *iconptr;
    const char *label;
    const char **buttons;
    const int numButtons;
};

struct hotspot
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t width = 0;
    uint16_t height = 0;
    const struct action *actions;
    uint16_t numActions;
};

void dumpConfig();
button *getButtonFromName(const char *name);
uint8_t getMenuIndexFromName(const char *name);
void setupMenu();
void updateMenu(bool renderMenu);
void runHotspot(int x, int y, int lastX, int lastY, int hotspotIndex, bool isFirstPress);
int checkHotspot(int x, int y);
void jumpToMenu(const char *menuName);
void normalizeTouch(uint16_t *x, uint16_t *y);
void setFakeTouch(int x, int y);
void processTouchLoop();
#endif