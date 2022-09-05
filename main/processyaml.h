#ifndef processyaml
#define processyaml

#include "yaml.h"
#include "theyaml.h"


#define MAX_VAR_LENGTH 40

struct entry
{
    char entry[MAX_VAR_LENGTH];
};

struct sequence
{
    char _name[MAX_VAR_LENGTH] = "";
    int length = 0;
    
    entry *entries = 0;
};

enum actionType
{
	ACTION_UNSET = -1,
    ACTION_HASERVICE = 0,
    ACTION_URL = 1,
    ACTION_NATIVE = 2,
    ACTION_JUMP = 3
};

struct action_haservice
{    
    char service[128];
    char target_json[128];
    char data_json[256];
};

struct action_url
{
    char url[256];
};

struct action_native
{
    char command[256];
    char data[256];
};
struct action_jump
{
    char destination[64];
};

struct action_all
{
	char data[512];
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
    union actionData data;
    enum actionType type;
};

struct button
{
    char _name[64] = "";
    char icon[MAX_VAR_LENGTH] = "";
    char label[MAX_VAR_LENGTH] = "";
    action* actions;
    int numActions = 0;
};

struct menu
{
    char _name[MAX_VAR_LENGTH] = "";
    char icon[MAX_VAR_LENGTH] = "";
    char label[MAX_VAR_LENGTH] = "";
    sequence *submenus;
    sequence *buttons;
};

void addMenu();
struct menu* getLastMenu();
void addSequence();
void expandSequence(struct sequence *_sequence);
struct sequence* getLastSequence();
void addButton();
struct action* getLastActionOnButton(struct button *_button);
void addActionToButton(struct button *_button);
struct button* getLastButton();
void getNextEvent(yaml_parser_t parser, yaml_event_t &event);
void processMappingStart_action(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);
void processMappingStart_button(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);
void processMappingStart_menu(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);
void* processMappingStart(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);
void* processSequenceStart(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);
void processSequenceEnd_menu();
void* processentry(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue);

int processMenu();
#endif
