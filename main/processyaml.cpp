#include "yaml.h"
#include "theyaml.h"
#include "processyaml.h"
#include <hardwareSerial.h>

void* (*scalarCallback) (yaml_parser_t, yaml_event_t, yaml_char_t*) = 0;

int mappingDepth = 0;

struct sequence *sequences = 0;
int numSequences = 0;

struct menu *menus = 0;
int numMenus = 0;
int lastMenuDepth = 0;

struct button *buttons = 0;
int numButtons = 0;
int lastButtonDepth = 0;

int lastActionDepth = 0;

yaml_char_t currentMapping[MAX_VAR_LENGTH] = { 0 };

void addMenu()
{    
    //Serial.printf("addMenu");//Serial.flush();
    numMenus++;
    struct menu *newMenus = (struct menu*)malloc(sizeof(struct menu)*numMenus);
    if (numMenus > 1)
    {
        memcpy(newMenus, menus, sizeof(menu)* ( numMenus - 1));
    }
    menus = newMenus;
    getLastMenu()->submenus = (sequence*)malloc(sizeof(sequence));
	getLastMenu()->submenus->length = 0;
    getLastMenu()->buttons = (sequence*)malloc(sizeof(sequence));
	getLastMenu()->buttons->length = 0;
}

struct menu* getLastMenu()
{
    //Serial.printf("getLastMenu");//Serial.flush();
    return &menus[numMenus - 1];
}

void addSequence()
{
    //Serial.printf("addSequence");//Serial.flush();
    numSequences++;
    struct sequence *newSequences = (struct sequence*)malloc(sizeof(struct sequence)*numSequences);
    if (numSequences > 1)
    {
        memcpy(newSequences, sequences, sizeof(sequence)* ( numSequences - 1));
    }
    sequences = newSequences;
	getLastSequence()->length = 0;
}

void expandSequence(struct sequence *_sequence)
{
    //Serial.printf("expandSequence");//Serial.flush(); 
    _sequence->length++;
	
    entry *newEntries = (entry *)malloc(sizeof(struct entry) * _sequence->length);
	//Serial.printf("\nmalloc(%i)\n",malloc(sizeof(struct entry) * _sequence->length));
	//Serial.printf("Result: %i\n",newEntries);
	//Serial.printf("sequence length: %i\n",_sequence->length);
	//Serial.printf("sizeof(struct entry)=  %i\n",sizeof(struct entry));
	//Serial.flush();
    if (_sequence->length > 1)
    {
		//Serial.printf("\nmemcpy(%p,%p,%i)\n",newEntries, _sequence->entries, MAX_VAR_LENGTH * ( _sequence->length - 1));//Serial.flush();
        memcpy(newEntries, _sequence->entries, MAX_VAR_LENGTH * ( _sequence->length - 1));
    }
    _sequence->entries = newEntries;
     //Serial.printf("exit expandSequence");//Serial.flush(); 
}

struct sequence* getLastSequence()
{
    //Serial.printf("getLastSequence");//Serial.flush();
    return &sequences[numSequences - 1];
}

void addButton()
{
    //Serial.printf("addButton");//Serial.flush();
    numButtons++;
    struct button *newButtons = (struct button*)malloc(sizeof(struct button)*numButtons);
    if (numButtons > 1)
    {
        memcpy(newButtons, buttons, sizeof(button)* ( numButtons - 1));
    }
    buttons = newButtons;
	getLastButton()->numActions = 0;
}

struct action* getLastActionOnButton(struct button *_button)
{
  //Serial.printf("getLastActionOnButton");//Serial.flush();
	return &_button->actions[_button->numActions-1];
}

void addActionToButton(struct button *_button)
{  
    //Serial.printf("addActionToButt");//Serial.flush();
    _button->numActions++;
    struct action *newActions = (struct action*)malloc(sizeof(struct action)*_button->numActions);
    if (_button->numActions > 1)
    {
        memcpy(newActions, _button->actions, sizeof(action)* ( _button->numActions - 1));
    }
    _button->actions = newActions;
	getLastActionOnButton(_button)->type = ACTION_UNSET;
	memset(getLastActionOnButton(_button)->data.all.data,0,sizeof(getLastActionOnButton(_button)->data.all.data));
	//Serial.printf("Clearing new action: memset(%p,%i,%i)",getLastActionOnButton(_button)->data.all.data,0,sizeof(getLastActionOnButton(_button)->data.all.data));
}

struct button* getLastButton()
{  
    //Serial.printf("getLastButton");//Serial.flush();
    return &buttons[numButtons - 1];
}

void getNextEvent(yaml_parser_t parser, yaml_event_t &event)
{  
    //Serial.printf("getNextEvent");//Serial.flush();
    yaml_parser_parse(&parser, &event);
    return;
}

void processMappingStart_action(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processMappingStart_action");//Serial.flush();
    bool justAdded = false;
    if (strcmp((char*)scalarValue,"Action") == 0)
    {
        lastActionDepth = mappingDepth;
        addActionToButton(getLastButton());
        justAdded = true;
    }
    
    if (mappingDepth <= lastActionDepth && !justAdded)
        lastActionDepth = 0;
	
	if (! lastActionDepth)
		return;
	
    struct action *lastAction = getLastActionOnButton(getLastButton());

    if (strcmp((char*)scalarValue,"Type") == 0)
    {
        getNextEvent(parser, event);
		char* value = (char*)event.data.scalar.value;
		if (strcmp(value,"HAService") == 0)
			lastAction->type = ACTION_HASERVICE;
		else if (strcmp(value,"Jump") == 0)
			lastAction->type = ACTION_JUMP;
		else if (strcmp(value,"Native") == 0)
			lastAction->type = ACTION_NATIVE;
		else if (strcmp(value,"URL") == 0)
			lastAction->type = ACTION_URL;
    }
    else if (strcmp((char*)scalarValue,"Service") == 0)
    {
		
        getNextEvent(parser, event);
        strcpy(lastAction->data.haservice.service, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Target_json") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.haservice.target_json, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Data_json") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.haservice.data_json, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Url") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.url.url, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Command") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.native.command, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Data") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.native.data, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Destination") == 0)
    {
        getNextEvent(parser, event);
        strcpy(lastAction->data.jump.destination, (char*)event.data.scalar.value);
    }
}

void processMappingStart_button(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processMappingStart_button");//Serial.flush();
    bool justAdded = false;
    if (strcmp((char*)scalarValue,"Button") == 0)
    {
        //Start a new menu
        lastButtonDepth = mappingDepth;
        addButton();
        justAdded = true;
    }
    
    if (mappingDepth <= lastButtonDepth && !justAdded)
        lastButtonDepth = 0;
    
    if (!lastButtonDepth)
    {
        return; // Not processing a menu
    }
    
    if (strcmp((char*)scalarValue,"Name") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastButton()->_name, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Icon") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastButton()->icon, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Label") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastButton()->label, (char*)event.data.scalar.value);
    }
}

void processMappingStart_menu(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processMappingStart_menu");//Serial.flush();
    bool justAdded = false;
    if (strcmp((char*)scalarValue,"Menu") == 0)
    {
        //Start a new menu
        lastMenuDepth = mappingDepth;
        addMenu();
        justAdded = true;
    }
    if (mappingDepth <= lastMenuDepth && !justAdded)
        lastMenuDepth = 0;
    if (!lastMenuDepth)
    {
        return; // Not processing a menu
    }
    

    if (strcmp((char*)scalarValue,"Name") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastMenu()->_name, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Icon") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastMenu()->icon, (char*)event.data.scalar.value);
    }
    else if (strcmp((char*)scalarValue,"Label") == 0)
    {
        getNextEvent(parser, event);
        strcpy(getLastMenu()->label, (char*)event.data.scalar.value);
    }
}

void* processMappingStart(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processMappingStart");//Serial.flush();
	processMappingStart_action(parser, event, scalarValue);
    processMappingStart_button(parser, event, scalarValue);
    processMappingStart_menu(parser, event, scalarValue);
    strcpy((char*) currentMapping, (char*) scalarValue);
    return 0;
}

void* processSequenceStart(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processSequenceStart");//Serial.flush();
    addSequence();
    strcpy(getLastSequence()->_name, (char*) currentMapping);
     return processentry(parser, event, scalarValue);
}

void processSequenceEnd_menu()
{  
    //Serial.printf("processSequenceEnd_menu");//Serial.flush();
    if (!lastMenuDepth)
        return; // Not processing a menu
    else if (strcmp((char*)currentMapping,"Buttons") == 0)
        getLastMenu()->buttons = getLastSequence();
    else if (strcmp((char*)currentMapping,"Submenus") == 0)
        getLastMenu()->submenus = getLastSequence();
}

void* processentry(yaml_parser_t parser, yaml_event_t event, yaml_char_t* scalarValue)
{  
    //Serial.printf("processentry");//Serial.flush();
    sequence *_sequence = getLastSequence();
    expandSequence(_sequence);
    //Serial.printf("return processentry");//Serial.flush();
    strcpy((char*)_sequence->entries[_sequence->length - 1].entry, (char*)scalarValue);
    //Serial.printf("processentry after strcopy");//Serial.flush();
    return (void*)&processentry;
    //return 0;
}

int processMenu()
{  
    //Serial.printf("processMenu");//Serial.flush();
    yaml_parser_t parser;
    yaml_document_t document;
    yaml_node_t* node;
    yaml_event_t event;
    
    yaml_parser_initialize(&parser) ;
    yaml_parser_set_input_string(&parser, (const unsigned char*) theYAML, strlen(theYAML));
    
    int done = 0;
    
    /* Read the event sequence. */
    int startSpaces = 0;

    //Serial.printf("Starting");
    //Serial.flush();
    
    int count = 0;
    while (!done) {
            //Serial.printf("Count: %i\n",count++);
            //Serial.flush();
    
            /* Get the next event. */
            if (!yaml_parser_parse(&parser, &event))
                    goto error;
            switch(event.type)
            {
            case YAML_NO_EVENT:
                break;
            case YAML_STREAM_START_EVENT:
                break;
            case YAML_STREAM_END_EVENT:
                break;
            case YAML_DOCUMENT_START_EVENT:
                break;
            case YAML_DOCUMENT_END_EVENT:
                break;
            case YAML_ALIAS_EVENT:
                break;
            case YAML_SCALAR_EVENT:
                if (scalarCallback)
                    scalarCallback = (void* (*)(yaml_parser_s, yaml_event_s, unsigned char*))scalarCallback(parser,event,event.data.scalar.value);
                break;
            case YAML_SEQUENCE_START_EVENT:
                scalarCallback = &processSequenceStart;
                break;
            case YAML_SEQUENCE_END_EVENT:
                processSequenceEnd_menu();
                scalarCallback = 0;
                break;
            case YAML_MAPPING_START_EVENT:
                mappingDepth++;
                scalarCallback = &processMappingStart;
                break;
            case YAML_MAPPING_END_EVENT:
                mappingDepth--;
                break;
            default:
                //Serial.printf("Unknown event %i!\n",event.type);
                break;
            }
    
            /*
                ...
                Process the event.
                ...
            */
    
            /* Are we finished? */
            done = (event.type == YAML_STREAM_END_EVENT);
    
            /* The application is responsible for destroying the event object. */
            yaml_event_delete(&event);
    
    }
    
    Serial.printf("Sequences:\n");
    for (int i=0;i<numSequences;i++)
    {
        Serial.printf("- %s: (Length %i)\n", sequences[i]._name,sequences[i].length);
        for (int j=0;j<sequences[i].length;j++)
        {
            Serial.printf("  - %s\n", sequences[i].entries[j].entry);
        }
    }
    
    Serial.printf("\n\nMenus: %i\n", numMenus);
    for (int i=0;i<numMenus;i++)
    {
        Serial.printf("Menu\n Name: %s\n Icon: %s\n Label: %s\n",menus[i]._name,menus[i].icon,menus[i].label);
        Serial.printf("  Submenus:\n");
        for (int j=0;j<menus[i].submenus->length;j++)
        {
            Serial.printf("    %s\n",menus[i].submenus->entries[j].entry);
        }
        Serial.printf("  Buttons:\n");
        for (int j=0;j<menus[i].buttons->length;j++)
        {
            Serial.printf("    %s\n",menus[i].buttons->entries[j].entry);
        }
    }
    
    Serial.printf("\n\nButtons: %i\n", numButtons);
    for (int i=0;i<numButtons;i++)
    {
        Serial.printf("Button\n Name: %s\n Icon: %s\n Label: %s\n",buttons[i]._name,buttons[i].icon,buttons[i].label);
		Serial.printf("  Actions: %i\n",buttons[i].numActions);
		for (int j=0;j<buttons[i].numActions;j++)
		{
			switch(buttons[i].actions[j].type)
			{
			case ACTION_HASERVICE:
				Serial.printf("    Call Home Assistant:\n");
				Serial.printf("      Service: %s\n",buttons[i].actions[j].data.haservice.service);
				Serial.printf("      target_json: %s\n",buttons[i].actions[j].data.haservice.target_json);
				Serial.printf("      data_json: %s\n",buttons[i].actions[j].data.haservice.data_json);
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

    Serial.printf("Done in %i milliseconds.",millis());
    
    /* Destroy the Parser object. */
    yaml_parser_delete(&parser);
    
    return 1;
    
    /* On error. */
    error:
    //Serial.printf("error\n");
    
    /* Destroy the Parser object. */
    yaml_parser_delete(&parser);
    

    return 0;
}
