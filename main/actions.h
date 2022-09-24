#ifndef actions_h
#define actions_h
#include <arduino.h>
#include <WiFi.h>
#include <ArduinoHttpClient.h>
#include "display.h"
#include "menu.h"
#include "secrets.h"

enum actionType
{
    ACTION_HASERVICE = 0,
    ACTION_URL = 1,
    ACTION_NATIVE = 2,
    ACTION_JUMP = 3,
    ACTION_UNSET = 4
};

struct action_haservice
{
    const char *service;
    const char *target_json;
    const char *data_json;
};

struct action_url
{
    const char *url;
};

struct action_native
{
    const char *command;
    const char *data;
};
struct action_jump
{
    const char *destination;
};

struct action_all
{
    const char *data;
};

union actionData
{
    struct action_haservice haservice;
    struct action_url url;
    struct action_native native;
    struct action_jump jump;
    struct action_all all;
};

struct action
{
    const union actionData data;
    const enum actionType type;
};

struct action_task
{
    const struct action* actions;
    uint32_t numActions;
    uint16_t x;
    uint16_t y;
};

#define MAX_TASKS 6 //Max number of concurent actions to run, additional actions beyond this will block the UI.

void harequest(const char *service, const char *target_json, const char *data_json);
void httpRequest();
void ha_light_set_color(bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness);
void ha_light_send_state(const char *_lightName, bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness);
void ha_light_add_subtract_color(bool subtract, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness);
void ha_light_select(const char* name);
void taskDoActions(const action* pActions, uint8_t numActions,uint16_t x, uint16_t y);
void taskDoActionsDo(void *params);
void doActions(const action* pActions, uint8_t numActions,uint16_t x, uint16_t y);
#endif