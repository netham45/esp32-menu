#ifndef structs
#define structs
#include <arduino.h>

struct sequence
{
    const char* _name;
    const int length;
    
    const char **entries;
};

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
    const char* service;
    const char* target_json;
    const char* data_json;
};

struct action_url
{
    const char* url;
};

struct action_native
{
    const char* command;
    const char* data;
};
struct action_jump
{
    const char* destination;
};

struct action_all
{
	  const char* data;
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

struct button
{
    const char* _name;
    const char* icon;
    const unsigned char* iconptr;
    const char* label;
    const int numActions;
    const struct action *actions;
};

struct menu
{
    const char* _name;
    const char* icon;
    const unsigned char* iconptr;
    const char* label;
    const struct sequence buttons;
};

#endif
