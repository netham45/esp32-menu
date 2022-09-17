#include "structs.h"
#include "display.h"
#include "actions.h"
#ifndef menu_h
#define menu_h
#define MENU_ROWS 4
#define MENU_COLUMNS 6
void printShit();
button *getButtonFromName(const char *name);
uint8_t getMenuIndexFromName(const char *name);
void drawMenu();
void doButtonActions(uint8_t buttonIndex);
#endif
