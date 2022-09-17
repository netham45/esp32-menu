#!/usr/bin/python3.9
import yaml

print('#include "graphics/graphics.h"')
print('#include "structs.h"')

with open('theyaml.yaml', 'r') as file:
  yamldata = yaml.safe_load(file)

entry_count = 0
sequence_count = 0
menu_count = 0
menuList = ""
for Menu in yamldata["Menus"]:
  menuList = menuList + "menu_{},".format(menu_count)
  buttonList = ""
  button_count = 0
  if 'Buttons' in Menu.keys():
    button_count = len(Menu['Buttons'])
    for Button in Menu['Buttons']:
      buttonList = buttonList + '"{}",'.format(Button)
  print("const char* entries_{}[] = {{ {} }};".format(entry_count,buttonList))
  print('const struct sequence sequence_{} = {{ "dummyname", {}, entries_{}}};'.format(sequence_count,button_count,entry_count))
  print('const struct menu menu_{} = {{"{}","{}",{}_bmp,"{}",sequence_{}}};'.format(menu_count,Menu['Name'],Menu['Icon'],Menu['Icon'],Menu['Label'],sequence_count))
  entry_count += 1
  sequence_count += 1
  menu_count += 1
print('struct menu menus[] = {{ {} }};'.format(menuList))
print('uint32_t numMenus = {};'.format(menu_count))


buttonList = ""
button_count = 0
action_data_count = 0
action_data_member_count = 0
action_count = 0
actions_count = 0;

for Button in yamldata["Buttons"]:
  buttonList = buttonList + "button_{},".format(button_count)
  button_action_count = 0
  actionList = ""
  if 'Actions' in Button.keys():
    for Action in Button['Actions']:
      actionList = actionList + "action_{},".format(action_count)
      actionType = ""
      if Action['Type'] == "HAService":
        actionType = "ACTION_HASERVICE"
        Data_json = ""
        Target_json = ""
        if "Data_json" in Action.keys():
          Data_json = Action['Data_json']
        if "Target_json" in Action.keys():
          Target_json = Action['Target_json']
        print('const struct action_haservice action_data_member_{} = {{ "{}", "{}", "{}" }};'.format(action_data_member_count,Action['Service'],Target_json,Data_json))
        print('const union actionData action_data_{} = {{.haservice = action_data_member_{} }};'.format(action_data_count,action_data_member_count))
      elif Action['Type'] == "Jump":
        actionType = "ACTION_JUMP"
        print('const struct action_jump action_data_member_{} = {{ "{}" }};'.format(action_data_member_count,Action['Destination']))
        print('const union actionData action_data_{} = {{.jump = action_data_member_{} }};'.format(action_data_count,action_data_member_count))
      elif Action['Type'] == "Native":
        actionType = "ACTION_NATIVE"
        Command = ""
        Data = ""
        if "Command" in Action.keys():
          Command = Action['Command']
        if "Data" in Action.keys():
          Data = Action['Data']
        print('const struct action_native action_data_member_{} = {{ "{}", "{}" }};'.format(action_data_member_count,Command,Data))
        print('const union actionData action_data_{} = {{.native = action_data_member_{} }};'.format(action_data_count,action_data_member_count))
      elif Action['Type'] == "URL":
        actionType = "ACTION_URL"
        print('const struct action_url action_data_member_{} = {{ "{}" }};'.format(action_data_member_count,Action['URL']))
        print('const union actionData action_data_{} = {{.url = action_data_member_{} }};'.format(action_data_count,action_data_member_count))
      else:
        print("ERROR: Unknown type {}".format(Action['Type']))
      print('const struct action action_{} = {{ action_data_{}, {} }};'.format(action_count,action_data_count,actionType))
      action_data_member_count += 1
      action_data_count += 1
      action_count += 1
      button_action_count += 1
  print('const struct action actions_{}[] = {{ {} }};'.format(actions_count,actionList))
  print('const struct button button_{} = {{ "{}", "{}", {}_bmp, "{}", {}, actions_{} }};'.format(button_count,Button['Name'],Button['Icon'],Button['Icon'],Button['Label'],button_action_count,actions_count))
  actions_count += 1
  button_count += 1
print('struct button buttons[] = {{ {} }};'.format(buttonList))
print('uint32_t numButtons = {};'.format(button_count))
