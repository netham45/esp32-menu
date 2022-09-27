#include "Adafruit_TSC2007.h"
#include <Wire.h>
#include <WiFi.h>
#include "display.h"
#include "menu.h"
#include "Esp.h"
#include "secrets.h"
#include "freertos/semphr.h"

void setupWifi()
{
    IPAddress local_IP(192, 168, 5, 136);
    IPAddress gateway(192, 168, 5, 3);
    IPAddress dns(192, 168, 5, 3);
    IPAddress subnet(255, 255, 255, 0);

    WiFi.mode(WIFI_STA);
    WiFi.config(local_IP, dns, gateway, subnet);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
}

bool isCleanBoot()
{
    return esp_sleep_get_wakeup_cause() != ESP_SLEEP_WAKEUP_EXT0;
}

void webServer(void *pParams)
{
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(10);
    }
    Serial.printf("webServer starting\n");
    // Current time
    unsigned long currentTime = millis();
    // Previous time
    unsigned long previousTime = 0;
    // Define timeout time in milliseconds (example: 2000ms = 2s)
    const long timeoutTime = 2000;
    WiFiServer server(80);
    String header;
    server.begin();
    while (1)
    {
        WiFiClient client = server.available(); // Listen for incoming clients

        if (client)
        { // If a new client connects,
            currentTime = millis();
            previousTime = currentTime;
            int curLineLength = 0;
            while (client.connected() && currentTime - previousTime <= timeoutTime)
            { // loop while the client's connected
                currentTime = millis();
                if (client.available())
                {                           // if there's bytes to read from the client,
                    char c = client.read(); // read a byte, then
                    header += c;
                    if (c == '\n')
                    { // if the byte is a newline character
                        // if the current line is blank, you got two newline characters in a row.
                        // that's the end of the client HTTP request, so send a response:
                        if (curLineLength == 0)
                        {
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");

                            // turns the GPIOs on and off
                            if (header.indexOf("GET /screenshot.bmp") >= 0)
                            {
                                client.println("Content-type:image/bmp");
                                client.println("Connection: close");
                                client.println();
                                sendFrameBufferAsBMP(client);
                            }
                            else if (header.indexOf("GET /click") >= 0)
                            {
                                int x = -1;
                                int y = -1;
                                int xystart = header.indexOf("XY=") + 3;
                                if (xystart < 0)
                                {
                                    client.println("Couldn't find XY= get param");
                                    break;
                                }
                                int xylen = header.substring(xystart).indexOf(" ");
                                String xy = header.substring(xystart, xystart + xylen);
                                sscanf(xy.c_str(), "%i,%i", &x, &y);

                                client.println("Content-type:text/plain");
                                client.println("Connection: close");
                                client.println();
                                setFakeTouch(x, y);
                            }
                            else
                            {
                                client.println("Content-type: text/html");
                                client.println("Connection: close");
                                
                                client.println(R"""(
<HTML>
  <HEAD>
    <TITLE>
	  ESP32 Menu Remote Control
	</TITLE>
  <SCRIPT TYPE='text/javascript'>
    var mouseclicked = false;
	var x = -1;
	var y = -1;
    window.onload = function() {
      document.getElementById('screenshot').addEventListener('mousedown', function (e) {
	      mouseclicked = true;
		  x = event.offsetX;
		  y = event.offsetY;
        }
	  );
      document.getElementById('screenshot').addEventListener('touchstart', function (e) {
	      mouseclicked = true;
          var bcr = e.target.getBoundingClientRect();
          x = parseInt(e.touches[0].clientX - bcr.x);
          y = parseInt(e.touches[0].clientY - bcr.y);
        }
	  );
	  document.getElementById('screenshot').addEventListener('mouseup', function (e) {
	      mouseclicked = false;
		  x = -1;
		  y = -1;
        }
	  );
      document.getElementById('screenshot').addEventListener('touchend', function (e) {
	      mouseclicked = false;
		  x = -1;
		  y = -1;
        }
	  );
      document.getElementById('screenshot').addEventListener('touchcancel', function (e) {
	      mouseclicked = false;
		  x = -1;
		  y = -1;
        }
	  );
	  document.getElementById('screenshot').addEventListener('mousemove', function (e) {
  	      if (mouseclicked)
	      {
			x = event.offsetX;
			y = event.offsetY;
		  }
        }
	  );
      document.getElementById('screenshot').addEventListener('touchmove', function (e) {
  	      if (mouseclicked)
	      {
            var bcr = e.target.getBoundingClientRect();
            x = parseInt(e.touches[0].clientX - bcr.x);
            y = parseInt(e.touches[0].clientY - bcr.y);
		  }
        }
	  );
	  setInterval(sendXY,100);
    }
	function sendXY()
	{
	  var xhr = new XMLHttpRequest();
	  var url = 'http://192.168.5.136/click?XY=' + x + ',' + y;
	  xhr.open('GET', url, true);
	  xhr.send();
	}
  </SCRIPT>
  </HEAD>
  <BODY>
    <IMG SRC='http://192.168.5.136/screenshot.bmp' ID='screenshot' DRAGGABLE='false' />
  </BODY>
</HTML>)""");
                                client.println();
                            }

                            // The HTTP response ends with another blank line
                            client.println();
                            // Break out of the while loop
                            break;
                        }
                        else
                        { // if you got a newline, then clear currentLine
                            curLineLength = 0;
                        }
                    }
                    else if (c != '\r')
                    { // if you got anything else but a carriage return character,
                        curLineLength++;
                    }
                }
            }
            // Clear the header variable
            header = "";
            // Close the connection
            client.stop();
        }
        delay(10);
    }
}

void setup()
{
    Serial.begin(115200);
    Wire.begin(22, 21);
    setupMenu(); // Call as early as possible to get initial touchscreen reading after wakeup
    setupWifi();
    esp_sleep_enable_ext0_wakeup(GPIO_NUM_33, 0);
    setupDisplay();
    xTaskCreatePinnedToCore(&webServer, "Action_Exec", 10000, 0, 1, NULL, 1);
    if (isCleanBoot()) // Render the menu on clean boot
    {
        updateMenu(true);
        updateDisplay();
    }
    while (1) // Testing
    {
        processTouchLoop();
    }
    esp_deep_sleep_start();
}

void loop()
{ // Shouldn't ever get here
    esp_deep_sleep_start();
}
