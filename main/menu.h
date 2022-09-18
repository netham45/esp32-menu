#include "structs.h"
#include "display.h"
#include "actions.h"
#ifndef menu_h
#define menu_h
#define MENU_ROWS 4
#define MENU_COLUMNS 6
#define TOPBUTTONCOUNT 6
void printShit();
button *getButtonFromName(const char *name);
uint8_t getMenuIndexFromName(const char *name);
void drawMenu();
void doButtonActions(button *_button);
void doButtonActionsMainIndex(uint8_t index);
void doButtonActionsTopBarIndex(uint8_t index);
#endif
