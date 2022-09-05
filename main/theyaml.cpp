const char* theYAML = R""""(
Menus:
  - Menu:
    - Name: Main Menu
    - Icon: mainmenu
    - Label: Main Menu
    - Submenus:
      - Xbox
      - Xbox One
      - Playstation 2
      - Genesis
      - Dreamcast
      - Switch
      - Wii U
      - SNES
      - N64
      - PC
      - TV
#      - Settings
  - Menu:
    - Name: Xbox
    - Icon: xbox
    - Label: Original Xbox
  - Menu:
    - Name: Xbox One
    - Icon: xboxone
    - Label: Xbox One
  - Menu:
    - Name: Playstation 2
    - Icon: ps2
    - Label: Playstation 2
    - Buttons:
      - RebootPS2
  - Menu:
    - Name: Genesis
    - Icon: genesis
    - Label: Genesis
    - Buttons:
      - RebootGenesis
  - Menu:
    - Name: Dreamcast
    - Icon: dreamcast
    - Label: Dreamcast
    - Buttons:
      - RebootDreamcast
  - Menu:
    - Name: Switch
    - Icon: switch
    - Label: Switch
  - Menu:
    - Name: Wii U
    - Icon: wiiu
    - Label: Wii U/Wii/Gamecube
  - Menu:
    - Name: SNES
    - Icon: snes
    - Label: Super Nintendo Entertainment System
    - Buttons:
      - RebootSNES
  - Menu:
    - Name: N64
    - Icon: n64
    - Label: Nintendo 64
    - Buttons:
      - RebootN64
  - Menu:
    - Name: PC
    - Icon: pc
    - Label: PC
  - Menu:
    - Name: TV
    - Icon: tv
    - Label: LG C1
    - Buttons:
      - TV Input Up
      - TV Input Down
      - TV Input Left
      - TV Input Right
      - TV Input OK
      - TV Input AppMenu
      - TV Input Back
      - TV Input Exit
  #- Menu:
  #  - Name: Settings
  #  - Icon: settings
  #  - Label: Settings
  #  - Buttons:
  #    - DFU mode
  #    - Clear RTC memory
  # - Wifi Settings
      
Buttons:
  - Button:
    - Name: Volume Up
    - Icon: volumeup
    - Label: Volume Up
    - Action: 
      - Type: HAService
      - Service: media_player.volume_up
      - target: '{"entity_id": "media_player.livingroom_tv"}'
  - Button:
    - Name: Volume Down
    - Icon: volumedown
    - Label: Volume Down
    - Action: 
      - Type: HAService
      - Service: media_player.volume_down
      - target: '{"entity_id": "media_player.livingroom_tv"}'
  - Button:
    - Name: Volume Mute
    - Icon: volumemute
    - Label: Volume Mute
    - Action: 
      - Type: HAService
      - Service: media_player.volume_mute
      - target: '{"entity_id": "media_player.livingroom_tv"}'    
  - Button:
    - Name: Main Menu
    - Icon: mainmenu
    - Label: Main Menu
    - Action:
      - Type: Jump
      - Destination: Main Menu
  - Button:
    - Name: RebootPS2
    - Icon: reboot
    - Label: Reboot Playstation 2
    - Action: 
      - Type: HAService
      - Service: script.rebootps2
  - Button:
    - Name: RebootGenesis
    - Icon: reboot
    - Label: Reboot Genesis
    - Action:
      - Type: HAService
      - Service: script.rebootgenesis
  - Button:
    - Name: RebootSNES
    - Icon: reboot
    - Label: Reboot Super Nintendo Entertainment System
    - Action: 
      - Type: HAService
      - Service: script.rebootsnes
  - Button:
    - Name: RebootN64
    - Icon: reboot
    - Label: Reboot Nintendo 64
    - Action: 
      - Type: HAService
      - Service: script.rebootn64
  - Button:
    - Name: RebootDreamcast
    - Icon: reboot
    - Label: Reboot Dreamcast
    - Action: 
      - Type: HAService
      - Service: script.rebootps2
  - Button:
    - Name: TV Input Up
    - Icon: uparrow
    - Label: ^
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "UP"}'
  - Button:
    - Name: TV Input Down
    - Icon: downarrow
    - Label: v
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "DOWN"}'
  - Button:
    - Name: TV Input Left
    - Icon: leftarrow
    - Label: <
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "LEFT"}'
  - Button:
    - Name: TV Input Right
    - Icon: rightarrow
    - Label: ">"
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "RIGHT"}'
  - Button:
    - Name: TV Input Enter
    - Icon: ok
    - Label: Enter
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "ENTER"}'
  - Button:
    - Name: TV Input AppMenu
    - Icon: tvappmenu
    - Label: App Menu
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "HOME"}'
  - Button:
    - Name: TV Input Back
    - Icon: back
    - Label: Back
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "BACK"}'
  - Button:
    - Name: TV Input Exit
    - Icon: exit
    - Label: Exit
    - Action: 
      - Type: HAService
      - Service: webostv.button
      - Data_json: '{"button": "EXIT"}'
)"""";