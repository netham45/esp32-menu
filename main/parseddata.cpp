#include "graphics.h"
#include "menu.h"
const char *buttons_0[] = {
    "Control TV",
    "Control Lights",
    "Control Computer",
    "Control Alarm",
    "Control Misc",
};
const struct menu menu_0 = {"Main Menu", "placeholder", &placeholder_bmp, "Main Menu", buttons_0, 5};
const char *buttons_1[] = {
    "TV Power Toggle",
    "TV Menu",
    "TV Input AppMenu",
    "TV Input Xbox",
    "TV Input Xbox One",
    "TV Input Genesis",
    "TV Input Dreamcast",
    "TV Input Switch",
    "TV Input Wii U",
    "TV Input SNES",
    "TV Input N64",
    "TV Input PC",
    "Refresh Display",
    "Control Lights",
};
const struct menu menu_1 = {"TV", "placeholder", &placeholder_bmp, "LG C1", buttons_1, 14};
const char *buttons_2[] = {
    "Livingroom Lights",
    "Kitchen Lights",
    "Master Bedroom Lights",
    "Outside Lights",
    "Lights Off",
    "Lights On",
    "Lights Red",
    "Lights Green",
    "Lights Blue",
    "Lights Yellow",
    "Lights Orange",
    "Lights Purple",
};
const struct menu menu_2 = {"Lights", "placeholder", &placeholder_bmp, "Control Lights", buttons_2, 12};
const char *buttons_3[] = {
    "Control TV",
    "Control Lights",
};
const struct menu menu_3 = {"Computer", "placeholder", &placeholder_bmp, "Control Computer", buttons_3, 2};
const char *buttons_4[] = {
    "Control TV",
    "Control Lights",
};
const struct menu menu_4 = {"Alarm", "placeholder", &placeholder_bmp, "Control Alarm", buttons_4, 2};
const char *buttons_5[] = {
    "Control TV",
    "Control Lights",
};
const struct menu menu_5 = {"Misc", "placeholder", &placeholder_bmp, "Control Misc", buttons_5, 2};
const char *buttons_6[] = {
    "Volume Up",
    "Volume Down",
    "Volume Mute",
    "Main Menu",
    "TV Input Right",
    "TV Input Up",
    "TV Input Enter",
    "TV Input Down",
    "TV Input Left",
    "TV Input Back",
    "TV Input Exit",
};
const struct menu menu_6 = {"TopBar", "placeholder", &placeholder_bmp, "TopBar", buttons_6, 11};
struct menu menus[] = {
    menu_0,
    menu_1,
    menu_2,
    menu_3,
    menu_4,
    menu_5,
    menu_6,
};
uint32_t numMenus = 7;
const struct action_haservice action_data_member_0 = {"media_player.volume_up", "", "{\"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_0 = {.haservice = action_data_member_0};
const struct action action_0 = {action_data_0, ACTION_HASERVICE};
const struct action actions_0[] = {
    action_0,
};
const struct button button_0 = {"Volume Up", "volumeup", &volumeup_bmp, "Volume Up", 1, actions_0};
const struct action_haservice action_data_member_1 = {"media_player.volume_down", "", "{\"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_1 = {.haservice = action_data_member_1};
const struct action action_1 = {action_data_1, ACTION_HASERVICE};
const struct action actions_1[] = {
    action_1,
};
const struct button button_1 = {"Volume Down", "volumedown", &volumedown_bmp, "Volume Down", 1, actions_1};
const struct action_haservice action_data_member_2 = {"webostv.button", "", "{\"button\": \"MUTE\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_2 = {.haservice = action_data_member_2};
const struct action action_2 = {action_data_2, ACTION_HASERVICE};
const struct action actions_2[] = {
    action_2,
};
const struct button button_2 = {"Volume Mute", "mute", &mute_bmp, "Volume Mute", 1, actions_2};
const struct action_jump action_data_member_3 = {"Main Menu"};
const union actionData action_data_3 = {.jump = action_data_member_3};
const struct action action_3 = {action_data_3, ACTION_JUMP};
const struct action actions_3[] = {
    action_3,
};
const struct button button_3 = {"Main Menu", "backarrow", &backarrow_bmp, "Main Menu", 1, actions_3};
const struct action_haservice action_data_member_4 = {"script.rebootps2", "", ""};
const union actionData action_data_4 = {.haservice = action_data_member_4};
const struct action action_4 = {action_data_4, ACTION_HASERVICE};
const struct action actions_4[] = {
    action_4,
};
const struct button button_4 = {"RebootPS2", "placeholder", &placeholder_bmp, "Reboot Playstation 2", 1, actions_4};
const struct action_haservice action_data_member_5 = {"script.rebootgenesis", "", ""};
const union actionData action_data_5 = {.haservice = action_data_member_5};
const struct action action_5 = {action_data_5, ACTION_HASERVICE};
const struct action actions_5[] = {
    action_5,
};
const struct button button_5 = {"RebootGenesis", "placeholder", &placeholder_bmp, "Reboot Genesis", 1, actions_5};
const struct action_haservice action_data_member_6 = {"script.rebootsnes", "", ""};
const union actionData action_data_6 = {.haservice = action_data_member_6};
const struct action action_6 = {action_data_6, ACTION_HASERVICE};
const struct action actions_6[] = {
    action_6,
};
const struct button button_6 = {"RebootSNES", "placeholder", &placeholder_bmp, "Reboot Super Nintendo Entertainment System", 1, actions_6};
const struct action_haservice action_data_member_7 = {"script.rebootn64", "", ""};
const union actionData action_data_7 = {.haservice = action_data_member_7};
const struct action action_7 = {action_data_7, ACTION_HASERVICE};
const struct action actions_7[] = {
    action_7,
};
const struct button button_7 = {"RebootN64", "placeholder", &placeholder_bmp, "Reboot Nintendo 64", 1, actions_7};
const struct action_haservice action_data_member_8 = {"script.rebootps2", "", ""};
const union actionData action_data_8 = {.haservice = action_data_member_8};
const struct action action_8 = {action_data_8, ACTION_HASERVICE};
const struct action actions_8[] = {
    action_8,
};
const struct button button_8 = {"RebootDreamcast", "placeholder", &placeholder_bmp, "Reboot Dreamcast", 1, actions_8};
const struct action_haservice action_data_member_9 = {"webostv.button", "", "{\"button\": \"UP\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_9 = {.haservice = action_data_member_9};
const struct action action_9 = {action_data_9, ACTION_HASERVICE};
const struct action actions_9[] = {
    action_9,
};
const struct button button_9 = {"TV Input Up", "uparrow", &uparrow_bmp, "Up", 1, actions_9};
const struct action_haservice action_data_member_10 = {"webostv.button", "", "{\"button\": \"DOWN\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_10 = {.haservice = action_data_member_10};
const struct action action_10 = {action_data_10, ACTION_HASERVICE};
const struct action actions_10[] = {
    action_10,
};
const struct button button_10 = {"TV Input Down", "downarrow", &downarrow_bmp, "Down", 1, actions_10};
const struct action_haservice action_data_member_11 = {"webostv.button", "", "{\"button\": \"LEFT\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_11 = {.haservice = action_data_member_11};
const struct action action_11 = {action_data_11, ACTION_HASERVICE};
const struct action actions_11[] = {
    action_11,
};
const struct button button_11 = {"TV Input Left", "leftarrow", &leftarrow_bmp, "Left", 1, actions_11};
const struct action_haservice action_data_member_12 = {"webostv.button", "", "{\"button\": \"RIGHT\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_12 = {.haservice = action_data_member_12};
const struct action action_12 = {action_data_12, ACTION_HASERVICE};
const struct action actions_12[] = {
    action_12,
};
const struct button button_12 = {"TV Input Right", "rightarrow", &rightarrow_bmp, "Right", 1, actions_12};
const struct action_haservice action_data_member_13 = {"webostv.button", "", "{\"button\": \"ENTER\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_13 = {.haservice = action_data_member_13};
const struct action action_13 = {action_data_13, ACTION_HASERVICE};
const struct action actions_13[] = {
    action_13,
};
const struct button button_13 = {"TV Input Enter", "backarrow", &backarrow_bmp, "Enter", 1, actions_13};
const struct action_haservice action_data_member_14 = {"webostv.button", "", "{\"button\": \"HOME\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_14 = {.haservice = action_data_member_14};
const struct action action_14 = {action_data_14, ACTION_HASERVICE};
const struct action actions_14[] = {
    action_14,
};
const struct button button_14 = {"TV Input AppMenu", "placeholder", &placeholder_bmp, "App Menu", 1, actions_14};
const struct action_haservice action_data_member_15 = {"webostv.button", "", "{\"button\": \"BACK\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_15 = {.haservice = action_data_member_15};
const struct action action_15 = {action_data_15, ACTION_HASERVICE};
const struct action actions_15[] = {
    action_15,
};
const struct button button_15 = {"TV Input Back", "backarrow", &backarrow_bmp, "Back", 1, actions_15};
const struct action_haservice action_data_member_16 = {"webostv.button", "", "{\"button\": \"EXIT\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_16 = {.haservice = action_data_member_16};
const struct action action_16 = {action_data_16, ACTION_HASERVICE};
const struct action actions_16[] = {
    action_16,
};
const struct button button_16 = {"TV Input Exit", "backarrow", &backarrow_bmp, "Exit", 1, actions_16};
const struct action_native action_data_member_17 = {"redraw", ""};
const union actionData action_data_17 = {.native = action_data_member_17};
const struct action action_17 = {action_data_17, ACTION_NATIVE};
const struct action actions_17[] = {
    action_17,
};
const struct button button_17 = {"Refresh Display", "placeholder", &placeholder_bmp, "Redraw", 1, actions_17};
const struct action_haservice action_data_member_18 = {"webostv.button", "", "{\"button\": \"POWER\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_18 = {.haservice = action_data_member_18};
const struct action action_18 = {action_data_18, ACTION_HASERVICE};
const struct action actions_18[] = {
    action_18,
};
const struct button button_18 = {"TV Power Toggle", "placeholder", &placeholder_bmp, "Power", 1, actions_18};
const struct action_haservice action_data_member_19 = {"webostv.button", "", "{\"button\": \"MENU\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_19 = {.haservice = action_data_member_19};
const struct action action_19 = {action_data_19, ACTION_HASERVICE};
const struct action actions_19[] = {
    action_19,
};
const struct button button_19 = {"TV Menu", "placeholder", &placeholder_bmp, "Menu", 1, actions_19};
const struct action_native action_data_member_20 = {"light_select", "light.livingroom_all_lights"};
const union actionData action_data_20 = {.native = action_data_member_20};
const struct action action_20 = {action_data_20, ACTION_NATIVE};
const struct action actions_20[] = {
    action_20,
};
const struct button button_20 = {"Livingroom Lights", "light_yellow_dim", &light_yellow_dim_bmp, "Livingroom", 1, actions_20};
const struct action_native action_data_member_21 = {"light_select", "light.kitchen_and_dining_room_all_lights"};
const union actionData action_data_21 = {.native = action_data_member_21};
const struct action action_21 = {action_data_21, ACTION_NATIVE};
const struct action actions_21[] = {
    action_21,
};
const struct button button_21 = {"Kitchen Lights", "light_yellow_dim", &light_yellow_dim_bmp, "Kitchen", 1, actions_21};
const struct action_native action_data_member_22 = {"light_select", "light.master_bedroom_all_lights"};
const union actionData action_data_22 = {.native = action_data_member_22};
const struct action action_22 = {action_data_22, ACTION_NATIVE};
const struct action actions_22[] = {
    action_22,
};
const struct button button_22 = {"Master Bedroom Lights", "light_yellow_dim", &light_yellow_dim_bmp, "Mstr Bdrm", 1, actions_22};
const struct action_native action_data_member_23 = {"light_select", "light.outside_lights"};
const union actionData action_data_23 = {.native = action_data_member_23};
const struct action action_23 = {action_data_23, ACTION_NATIVE};
const struct action actions_23[] = {
    action_23,
};
const struct button button_23 = {"Outside Lights", "light_yellow_dim", &light_yellow_dim_bmp, "Outside", 1, actions_23};
const struct action_native action_data_member_24 = {"light_off", ""};
const union actionData action_data_24 = {.native = action_data_member_24};
const struct action action_24 = {action_data_24, ACTION_NATIVE};
const struct action actions_24[] = {
    action_24,
};
const struct button button_24 = {"Lights Off", "light_black_dim", &light_black_dim_bmp, "Turn Off", 1, actions_24};
const struct action_native action_data_member_25 = {"light_on", ""};
const union actionData action_data_25 = {.native = action_data_member_25};
const struct action action_25 = {action_data_25, ACTION_NATIVE};
const struct action actions_25[] = {
    action_25,
};
const struct button button_25 = {"Lights On", "light_yellow_bright", &light_yellow_bright_bmp, "Turn On", 1, actions_25};
const struct action_native action_data_member_26 = {"light_add_rgbwb", "0020000000"};
const union actionData action_data_26 = {.native = action_data_member_26};
const struct action action_26 = {action_data_26, ACTION_NATIVE};
const struct action actions_26[] = {
    action_26,
};
const struct button button_26 = {"Lights Green Up", "light_green_bright", &light_green_bright_bmp, "Green +", 1, actions_26};
const struct action_native action_data_member_27 = {"light_subtract_rgbwb", "0020000000"};
const union actionData action_data_27 = {.native = action_data_member_27};
const struct action action_27 = {action_data_27, ACTION_NATIVE};
const struct action actions_27[] = {
    action_27,
};
const struct button button_27 = {"Lights Green Down", "green_slider", &green_slider_bmp, "Green -", 1, actions_27};
const struct action_native action_data_member_28 = {"light_add_rgbwb", "2000000000"};
const union actionData action_data_28 = {.native = action_data_member_28};
const struct action action_28 = {action_data_28, ACTION_NATIVE};
const struct action actions_28[] = {
    action_28,
};
const struct button button_28 = {"Lights Red Up", "red_slider", &red_slider_bmp, "Red +", 1, actions_28};
const struct action_native action_data_member_29 = {"light_subtract_rgbwb", "2000000000"};
const union actionData action_data_29 = {.native = action_data_member_29};
const struct action action_29 = {action_data_29, ACTION_NATIVE};
const struct action actions_29[] = {
    action_29,
};
const struct button button_29 = {"Lights Red Down", "light_red_dim", &light_red_dim_bmp, "Red -", 1, actions_29};
const struct action_native action_data_member_30 = {"light_add_rgbwb", "0000200000"};
const union actionData action_data_30 = {.native = action_data_member_30};
const struct action action_30 = {action_data_30, ACTION_NATIVE};
const struct action actions_30[] = {
    action_30,
};
const struct button button_30 = {"Lights Blue Up", "blue_slider", &blue_slider_bmp, "Blue +", 1, actions_30};
const struct action_native action_data_member_31 = {"light_subtract_rgbwb", "0000200000"};
const union actionData action_data_31 = {.native = action_data_member_31};
const struct action action_31 = {action_data_31, ACTION_NATIVE};
const struct action actions_31[] = {
    action_31,
};
const struct button button_31 = {"Lights Blue Down", "light_blue_dim", &light_blue_dim_bmp, "Blue -", 1, actions_31};
const struct action_native action_data_member_32 = {"light_add_rgbwb", "0000002000"};
const union actionData action_data_32 = {.native = action_data_member_32};
const struct action action_32 = {action_data_32, ACTION_NATIVE};
const struct action actions_32[] = {
    action_32,
};
const struct button button_32 = {"Lights White Up", "colortemp_slider", &colortemp_slider_bmp, "White +", 1, actions_32};
const struct action_native action_data_member_33 = {"light_subtract_rgbwb", "0000002000"};
const union actionData action_data_33 = {.native = action_data_member_33};
const struct action action_33 = {action_data_33, ACTION_NATIVE};
const struct action actions_33[] = {
    action_33,
};
const struct button button_33 = {"Lights White Down", "light_yellow_dim", &light_yellow_dim_bmp, "White -", 1, actions_33};
const struct action_native action_data_member_34 = {"light_add_rgbwb", "0000000020"};
const union actionData action_data_34 = {.native = action_data_member_34};
const struct action action_34 = {action_data_34, ACTION_NATIVE};
const struct action actions_34[] = {
    action_34,
};
const struct button button_34 = {"Lights Brightness Up", "light_yellow_bright", &light_yellow_bright_bmp, "Brghtnes +", 1, actions_34};
const struct action_native action_data_member_35 = {"light_subtract_rgbwb", "0000000020"};
const union actionData action_data_35 = {.native = action_data_member_35};
const struct action action_35 = {action_data_35, ACTION_NATIVE};
const struct action actions_35[] = {
    action_35,
};
const struct button button_35 = {"Lights Brightness Down", "light_yellow_dim", &light_yellow_dim_bmp, "Brghtnes -", 1, actions_35};
const struct action_native action_data_member_36 = {"light_rgbw", "FF000000"};
const union actionData action_data_36 = {.native = action_data_member_36};
const struct action action_36 = {action_data_36, ACTION_NATIVE};
const struct action actions_36[] = {
    action_36,
};
const struct button button_36 = {"Lights Red", "light_red_bright", &light_red_bright_bmp, "Red", 1, actions_36};
const struct action_native action_data_member_37 = {"light_rgbw", "00FF0000"};
const union actionData action_data_37 = {.native = action_data_member_37};
const struct action action_37 = {action_data_37, ACTION_NATIVE};
const struct action actions_37[] = {
    action_37,
};
const struct button button_37 = {"Lights Green", "light_green_bright", &light_green_bright_bmp, "Green", 1, actions_37};
const struct action_native action_data_member_38 = {"light_rgbw", "0000FF00"};
const union actionData action_data_38 = {.native = action_data_member_38};
const struct action action_38 = {action_data_38, ACTION_NATIVE};
const struct action actions_38[] = {
    action_38,
};
const struct button button_38 = {"Lights Blue", "light_blue_bright", &light_blue_bright_bmp, "Blue", 1, actions_38};
const struct action_native action_data_member_39 = {"light_rgbw", "FFFF0000"};
const union actionData action_data_39 = {.native = action_data_member_39};
const struct action action_39 = {action_data_39, ACTION_NATIVE};
const struct action actions_39[] = {
    action_39,
};
const struct button button_39 = {"Lights Yellow", "light_yellow_bright", &light_yellow_bright_bmp, "Yellow", 1, actions_39};
const struct action_native action_data_member_40 = {"light_rgbw", "FF8C0000"};
const union actionData action_data_40 = {.native = action_data_member_40};
const struct action action_40 = {action_data_40, ACTION_NATIVE};
const struct action actions_40[] = {
    action_40,
};
const struct button button_40 = {"Lights Orange", "light_yellow_bright", &light_yellow_bright_bmp, "Orange", 1, actions_40};
const struct action_native action_data_member_41 = {"light_rgbw", "88008800"};
const union actionData action_data_41 = {.native = action_data_member_41};
const struct action action_41 = {action_data_41, ACTION_NATIVE};
const struct action actions_41[] = {
    action_41,
};
const struct button button_41 = {"Lights Purple", "light_blue_bright", &light_blue_bright_bmp, "Purple", 1, actions_41};
const struct action_native action_data_member_42 = {"red_slider", ""};
const union actionData action_data_42 = {.native = action_data_member_42};
const struct action action_42 = {action_data_42, ACTION_NATIVE};
const struct action actions_42[] = {
    action_42,
};
const struct button button_42 = {"red_slider", "red_slider", &red_slider_bmp, "Red", 1, actions_42};
const struct action_native action_data_member_43 = {"green_slider", ""};
const union actionData action_data_43 = {.native = action_data_member_43};
const struct action action_43 = {action_data_43, ACTION_NATIVE};
const struct action actions_43[] = {
    action_43,
};
const struct button button_43 = {"green_slider", "green_slider", &green_slider_bmp, "Green", 1, actions_43};
const struct action_native action_data_member_44 = {"blue_slider", ""};
const union actionData action_data_44 = {.native = action_data_member_44};
const struct action action_44 = {action_data_44, ACTION_NATIVE};
const struct action actions_44[] = {
    action_44,
};
const struct button button_44 = {"blue_slider", "blue_slider", &blue_slider_bmp, "Blue", 1, actions_44};
const struct action_native action_data_member_45 = {"white_slider", ""};
const union actionData action_data_45 = {.native = action_data_member_45};
const struct action action_45 = {action_data_45, ACTION_NATIVE};
const struct action actions_45[] = {
    action_45,
};
const struct button button_45 = {"white_slider", "yellow_slider", &yellow_slider_bmp, "White", 1, actions_45};
const struct action_native action_data_member_46 = {"brightness_slider", ""};
const union actionData action_data_46 = {.native = action_data_member_46};
const struct action action_46 = {action_data_46, ACTION_NATIVE};
const struct action actions_46[] = {
    action_46,
};
const struct button button_46 = {"brightness_slider", "yellow_slider", &yellow_slider_bmp, "Brightness", 1, actions_46};
const struct action_native action_data_member_47 = {"colortemp_slider", ""};
const union actionData action_data_47 = {.native = action_data_member_47};
const struct action action_47 = {action_data_47, ACTION_NATIVE};
const struct action actions_47[] = {
    action_47,
};
const struct button button_47 = {"colortemp_slider", "colortemp_slider", &colortemp_slider_bmp, "Temperature", 1, actions_47};
const struct action_jump action_data_member_48 = {"Computer"};
const union actionData action_data_48 = {.jump = action_data_member_48};
const struct action action_48 = {action_data_48, ACTION_JUMP};
const struct action actions_48[] = {
    action_48,
};
const struct button button_48 = {"Control Computer", "placeholder", &placeholder_bmp, "Computer", 1, actions_48};
const struct action_jump action_data_member_49 = {"Alarm"};
const union actionData action_data_49 = {.jump = action_data_member_49};
const struct action action_49 = {action_data_49, ACTION_JUMP};
const struct action actions_49[] = {
    action_49,
};
const struct button button_49 = {"Control Alarm", "placeholder", &placeholder_bmp, "Alarm", 1, actions_49};
const struct action_jump action_data_member_50 = {"Misc"};
const union actionData action_data_50 = {.jump = action_data_member_50};
const struct action action_50 = {action_data_50, ACTION_JUMP};
const struct action actions_50[] = {
    action_50,
};
const struct button button_50 = {"Control Misc", "placeholder", &placeholder_bmp, "Misc", 1, actions_50};
const struct action_jump action_data_member_51 = {"Lights"};
const union actionData action_data_51 = {.jump = action_data_member_51};
const struct action action_51 = {action_data_51, ACTION_JUMP};
const struct action actions_51[] = {
    action_51,
};
const struct button button_51 = {"Control Lights", "light_yellow_bright", &light_yellow_bright_bmp, "Lights", 1, actions_51};
const struct action_jump action_data_member_52 = {"TV"};
const union actionData action_data_52 = {.jump = action_data_member_52};
const struct action action_52 = {action_data_52, ACTION_JUMP};
const struct action actions_52[] = {
    action_52,
};
const struct button button_52 = {"Control TV", "placeholder", &placeholder_bmp, "TV", 1, actions_52};
const struct action_haservice action_data_member_53 = {"script.switch_tv_to_hdmi_2_and_receiver_to_hdmi_3", "", ""};
const union actionData action_data_53 = {.haservice = action_data_member_53};
const struct action action_53 = {action_data_53, ACTION_HASERVICE};
const struct action actions_53[] = {
    action_53,
};
const struct button button_53 = {"TV Input Xbox", "xbox", &xbox_bmp, "Xbox/PS2", 1, actions_53};
const struct action_haservice action_data_member_54 = {"script.switch_tv_to_hdmi_2_and_receiver_to_hdmi_4", "", ""};
const union actionData action_data_54 = {.haservice = action_data_member_54};
const struct action action_54 = {action_data_54, ACTION_HASERVICE};
const struct action actions_54[] = {
    action_54,
};
const struct button button_54 = {"TV Input Xbox One", "xboxone", &xboxone_bmp, "Xbox One", 1, actions_54};
const struct action_haservice action_data_member_55 = {"script.switch_tv_to_hdmi_2_and_receiver_to_hdmi_3", "", ""};
const union actionData action_data_55 = {.haservice = action_data_member_55};
const struct action action_55 = {action_data_55, ACTION_HASERVICE};
const struct action actions_55[] = {
    action_55,
};
const struct button button_55 = {"TV Input PS2", "playstation", &playstation_bmp, "PS2", 1, actions_55};
const struct action_haservice action_data_member_56 = {"script.switch_tv_to_hdmi_3_retrotink_and_av_switcher_to_av4", "", ""};
const union actionData action_data_56 = {.haservice = action_data_member_56};
const struct action action_56 = {action_data_56, ACTION_HASERVICE};
const struct action actions_56[] = {
    action_56,
};
const struct button button_56 = {"TV Input Genesis", "sega", &sega_bmp, "Genesis", 1, actions_56};
const struct action_haservice action_data_member_57 = {"script.switch_tv_to_hdmi_3_retrotink_and_av_switcher_to_av3", "", ""};
const union actionData action_data_57 = {.haservice = action_data_member_57};
const struct action action_57 = {action_data_57, ACTION_HASERVICE};
const struct action actions_57[] = {
    action_57,
};
const struct button button_57 = {"TV Input Dreamcast", "dreamcast", &dreamcast_bmp, "Dreamcast", 1, actions_57};
const struct action_haservice action_data_member_58 = {"script.switch_tv_to_hdmi_2_and_receiver_to_hdmi_1", "", ""};
const union actionData action_data_58 = {.haservice = action_data_member_58};
const struct action action_58 = {action_data_58, ACTION_HASERVICE};
const struct action actions_58[] = {
    action_58,
};
const struct button button_58 = {"TV Input Switch", "switch", &switch_bmp, "Switch", 1, actions_58};
const struct action_haservice action_data_member_59 = {"script.switch_tv_to_hdmi_2_and_receiver_to_hdmi_2", "", ""};
const union actionData action_data_59 = {.haservice = action_data_member_59};
const struct action action_59 = {action_data_59, ACTION_HASERVICE};
const struct action actions_59[] = {
    action_59,
};
const struct button button_59 = {"TV Input Wii U", "wiiu", &wiiu_bmp, "Wii U", 1, actions_59};
const struct action_haservice action_data_member_60 = {"script.switch_tv_to_hdmi_3_retrotink_and_av_switcher_to_av2", "", ""};
const union actionData action_data_60 = {.haservice = action_data_member_60};
const struct action action_60 = {action_data_60, ACTION_HASERVICE};
const struct action actions_60[] = {
    action_60,
};
const struct button button_60 = {"TV Input SNES", "snes", &snes_bmp, "SNES", 1, actions_60};
const struct action_haservice action_data_member_61 = {"script.switch_tv_to_hdmi_3_retrotink_and_av_switcher_to_av1", "", ""};
const union actionData action_data_61 = {.haservice = action_data_member_61};
const struct action action_61 = {action_data_61, ACTION_HASERVICE};
const struct action actions_61[] = {
    action_61,
};
const struct button button_61 = {"TV Input N64", "n64", &n64_bmp, "N64", 1, actions_61};
const struct action_haservice action_data_member_62 = {"media_player.select_source", "", "{\"source\": \"PC\", \"entity_id\": \"media_player.livingroom_tv\"}"};
const union actionData action_data_62 = {.haservice = action_data_member_62};
const struct action action_62 = {action_data_62, ACTION_HASERVICE};
const struct action actions_62[] = {
    action_62,
};
const struct button button_62 = {"TV Input PC", "windows", &windows_bmp, "PC", 1, actions_62};
struct button buttons[] = {
    button_0,
    button_1,
    button_2,
    button_3,
    button_4,
    button_5,
    button_6,
    button_7,
    button_8,
    button_9,
    button_10,
    button_11,
    button_12,
    button_13,
    button_14,
    button_15,
    button_16,
    button_17,
    button_18,
    button_19,
    button_20,
    button_21,
    button_22,
    button_23,
    button_24,
    button_25,
    button_26,
    button_27,
    button_28,
    button_29,
    button_30,
    button_31,
    button_32,
    button_33,
    button_34,
    button_35,
    button_36,
    button_37,
    button_38,
    button_39,
    button_40,
    button_41,
    button_42,
    button_43,
    button_44,
    button_45,
    button_46,
    button_47,
    button_48,
    button_49,
    button_50,
    button_51,
    button_52,
    button_53,
    button_54,
    button_55,
    button_56,
    button_57,
    button_58,
    button_59,
    button_60,
    button_61,
    button_62,
};
uint32_t numButtons = 63;
