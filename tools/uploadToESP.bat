@echo off
cd /d "C:\Users\netham45\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.4\tools"
python espota.py -i 192.168.5.136 -f C:\arduino\build\main.ino.bin -P 8266 -p 3232 -I 192.168.0.50
pause