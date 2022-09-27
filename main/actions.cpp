#include "actions.h"
#include "math.h"
#include "freertos/semphr.h"
SemaphoreHandle_t lightOptions_mux = 0;
RTC_DATA_ATTR int16_t light_red = 255, light_green = 255, light_blue = 255, light_white = 255, light_brightness = 255;
RTC_DATA_ATTR char lightName[64] = {0};

SemaphoreHandle_t taskcount_mux = 0;
int taskcount = 0;

double clamp(double x, double upper, double lower)
{
    return min(upper, max(x, lower));
}

void harequest(const char *service, const char *target_json, const char *data_json)
{
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, "192.168.5.107", 8123);
    uint32_t maxwait = 100; // 10ms per tick
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(10);
        if (!maxwait--)
        {
            Serial.println("Failed to connect to wifi");
            return;
        }
    }
    char servicepath[128] = {0};
    sprintf(servicepath, "/api/services/%s", service);
    for (int i = 0; servicepath[i]; i++)
        if (servicepath[i] == '.')
            servicepath[i] = '/';
    uint8_t counter = 0;

    http.beginRequest();
    http.setHttpResponseTimeout(1000);
    uint8_t result = http.post(servicepath);
    if (result != 0)
    {
        Serial.printf("Request timeout\n");
        return;
    }
    http.setHttpResponseTimeout(4000);
    http.setTimeout(100);
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("authorization", HA_TOKEN);
    http.sendHeader("Content-Length", strlen(data_json));
    http.beginBody();
    http.print(data_json);
    http.endRequest();
}


void ha_light_send_rgbwb(const char *_lightName, bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
{
    char service[32];
    char data[128];
    if (turnOn)
    {
        strcpy(service, "light.turn_on");
        sprintf(data, "{\"entity_id\": \"%s\", \"rgbw_color\": [%i, %i, %i, %i], \"brightness\":%i} ", _lightName, red, green, blue, white, brightness);
    }
    else
    {
        strcpy(service, "light.turn_off");
        sprintf(data, "{\"entity_id\": \"%s\"} ", _lightName);
    }
    harequest(service, "", data);
}

void ha_light_send_colortemp(const char *_lightName, uint16_t colorTemp, uint8_t brightness)
{
    char service[32];
    char data[128];
    strcpy(service, "light.turn_on");
    sprintf(data, "{\"entity_id\": \"%s\", \"color_temp\": %i, \"brightness\":%i} ", _lightName, colorTemp, brightness);

    harequest(service, "", data);
}

void ha_light_send_brightness(const char *_lightName, uint8_t brightness)
{
    char service[32];
    char data[128];
    strcpy(service, "light.turn_on");
    sprintf(data, "{\"entity_id\": \"%s\", \"brightness\":%i} ", _lightName, brightness);

    harequest(service, "", data);
}

void ha_light_set_color(bool turnOn, int red, int green, int blue, int white, int brightness)
{
    while (!xSemaphoreTake(lightOptions_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    char _lightName[64];
    strcpy(_lightName, lightName);
    if (red != -1)
        light_red = red;
    if (green != -1)
        light_green = green;
    if (blue != -1)
        light_blue = blue;
    if (white != -1)
        light_white = white;
    if (brightness != -1)
        light_brightness = brightness;
    red = light_red;
    green = light_green;
    blue = light_blue;
    white = light_white;
    brightness = light_brightness;
    xSemaphoreGive(lightOptions_mux);
    ha_light_send_rgbwb(_lightName, turnOn, red, green, blue, white, brightness);
}


void ha_light_set_colortemp(int colortemp)
{
    while (!xSemaphoreTake(lightOptions_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    char _lightName[64];
    strcpy(_lightName, lightName);
    int brightness = light_brightness;
    xSemaphoreGive(lightOptions_mux);
    ha_light_send_colortemp(_lightName, colortemp, brightness);
}

void ha_light_set_brightness(int brightness)
{
    brightness=clamp(brightness,255,0);
    while (!xSemaphoreTake(lightOptions_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    char _lightName[64];
    strcpy(_lightName, lightName);
    light_brightness = brightness;
    xSemaphoreGive(lightOptions_mux);
    ha_light_send_brightness(_lightName, brightness);
}

void ha_light_add_subtract_color(bool subtract, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
{
    while (!xSemaphoreTake(lightOptions_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    if (!subtract)
    {
        light_red += red;
        light_green += green;
        light_blue += blue;
        light_white += white;
        light_brightness += brightness;
    }
    else
    {
        light_red -= red;
        light_green -= green;
        light_blue -= blue;
        light_white -= white;
        light_brightness -= brightness;
    }
    light_red = clamp(light_red, 255, 0);
    light_green = clamp(light_green, 255, 0);
    light_blue = clamp(light_blue, 255, 0);
    light_white = clamp(light_white, 255, 0);
    light_brightness = clamp(light_brightness, 255, 0);
    red = light_red;
    green = light_green;
    blue = light_blue;
    white = light_white;
    brightness = light_brightness;
    char _lightName[64];
    strcpy(_lightName, lightName);
    xSemaphoreGive(lightOptions_mux);
    ha_light_send_rgbwb(_lightName, true, red, green, blue, white, brightness);
}

void ha_light_select(const char *name)
{
    while (!xSemaphoreTake(lightOptions_mux, portMAX_DELAY) == pdTRUE)
    {
        vTaskDelay(10);
    }
    strcpy(lightName, name);
    xSemaphoreGive(lightOptions_mux);
}

void httpRequest()
{
    WiFiClient wifi;
    HttpClient http = HttpClient(wifi, "192.168.5.107", 8123);
    Serial.println("\nhttpRequest");
    uint32_t maxwait = 100; // 10ms per tick
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(10);
        if (!maxwait--)
        {
            Serial.println("Failed to connect to wifi");
            return;
        }
    }
    char postData[512] = "{\"entity_id\": \"light.livingroom_all_lights\"}";
    http.beginRequest();
    http.post("/api/services/light/toggle");
    http.sendHeader("Content-Type", "application/json");
    http.sendHeader("authorization", HA_TOKEN);
    http.sendHeader("Content-Length", strlen(postData));
    http.beginBody();
    http.print(postData);
    http.endRequest();
    Serial.printf("post status: %i\n", http.responseStatusCode());
}

void taskDoActions(const action *pActions, uint8_t numActions, int x, int y, int lastX, int lastY, bool isFirstClick)
{
    if (!lightOptions_mux)
    {
        lightOptions_mux = xSemaphoreCreateMutex();
    }
    if (!taskcount_mux)
    {
        taskcount_mux = xSemaphoreCreateMutex();
    }
    assert(lightOptions_mux);
    assert(taskcount_mux);
    bool keepwaiting = true;
    while (keepwaiting)
    {
        while (!xSemaphoreTake(taskcount_mux, portMAX_DELAY) == pdTRUE)
        {
            vTaskDelay(10);
        }
        if (taskcount < MAX_TASKS)
        {
            taskcount++;
            keepwaiting = false;
        }
        xSemaphoreGive(taskcount_mux);
        delay(10);
    }
    struct action_task *taskParams = (action_task *)malloc(sizeof(action_task));
    taskParams->actions = pActions;
    taskParams->numActions = numActions;
    taskParams->x = x;
    taskParams->y = y;
    taskParams->lastX = lastX;
    taskParams->lastY = lastY;
    taskParams->isFirstClick = isFirstClick;
    xTaskCreatePinnedToCore(&taskDoActionsDo, "Action_Exec", 10000, taskParams, 1, NULL, 1);
}

void taskDoActionsDo(void *pParams)
{
    struct action_task *taskParams = (action_task *)pParams;
    doActions(taskParams->actions, taskParams->numActions, taskParams->x, taskParams->y, taskParams->lastX, taskParams->lastY, taskParams->isFirstClick);
    free(taskParams);
    while (xSemaphoreTake(taskcount_mux, portMAX_DELAY) != pdTRUE)
    {
        vTaskDelay(10);
    }
    taskcount--;
    assert(taskcount > -1);
    xSemaphoreGive(taskcount_mux);
    vTaskDelete(NULL);
}

void doActions(const action *pActions, uint8_t numActions, int x, int y,  int lastX, int lastY, bool isFirstClick) // x,y are relative to the upper left corner of the hitbox
{
    for (int i = 0; i < numActions; i++)
    {
        switch (pActions[i].type)
        {
        case ACTION_HASERVICE:
            Serial.printf("Running HAService\n");
            char service[64];
            char target_json[64];
            char data_json[64];
            strcpy(service, pActions[i].data.haservice.service);
            strcpy(target_json, pActions[i].data.haservice.target_json);
            strcpy(data_json, pActions[i].data.haservice.data_json);
            harequest(service,
                      target_json,
                      data_json);
            break;
        case ACTION_URL:
            Serial.printf("Running URL\n");
            // httprequest(_action.data.url.url);
            break;
        case ACTION_NATIVE:
            Serial.printf("Running Native Command %s\n", pActions[i].data.native.command);
            if (strcmp("redraw", pActions[i].data.native.command) == 0)
            {
                if (isFirstClick)
                    updateDisplay();
            }
            else if (strcmp("light_select", pActions[i].data.native.command) == 0)
            {
                ha_light_select(pActions[i].data.native.data);
            }
            // void ha_light_set_state(const char* _lightName, bool turnOn, uint8_t red, uint8_t green, uint8_t blue, uint8_t white, uint8_t brightness)
            else if (strcmp("light_off", pActions[i].data.native.command) == 0)
            {
                ha_light_set_color(false, 0, 0, 0, 0, 0);
            }
            else if (strcmp("light_on", pActions[i].data.native.command) == 0)
            {
                ha_light_set_color(true, 0, 0, 0, 0xFF, 0xFF);
            }
            else if (strcmp("light_rgbw", pActions[i].data.native.command) == 0)
            {
                uint32_t hexval;
                uint8_t *rgbw = (uint8_t *)&hexval;
                sscanf(pActions[i].data.native.data, "%x", &hexval);

                ha_light_set_color(true, rgbw[3], rgbw[2], rgbw[1], rgbw[0], 0xFF);
            }
            else if (strcmp("light_add_rgbwb", pActions[i].data.native.command) == 0)
            {
                Serial.printf("data: %s\n", pActions[i].data.native.data);
                char top8[9] = {0};
                char bottom2[3] = {0};
                memcpy(top8, pActions[i].data.native.data, 8);
                memcpy(bottom2, pActions[i].data.native.data + 8, 2);
                uint32_t lrgbw;
                uint8_t *rgbw = (uint8_t *)&lrgbw;
                uint32_t ibrightness;
                uint8_t *brightness = (uint8_t *)&ibrightness;
                sscanf(top8, "%x", &lrgbw);
                sscanf(bottom2, "%x", &ibrightness);
                Serial.printf("(add) Got rgbwb: %x %x %x %x %x\n", rgbw[3], rgbw[2], rgbw[1], rgbw[0], brightness[0]);
                ha_light_add_subtract_color(false, rgbw[3], rgbw[2], rgbw[1], rgbw[0], brightness[0]);
            }
            else if (strcmp("light_subtract_rgbwb", pActions[i].data.native.command) == 0)
            {
                char top8[9] = {0};
                char bottom2[3] = {0};
                memcpy(top8, pActions[i].data.native.data, 8);
                memcpy(bottom2, pActions[i].data.native.data + 8, 2);
                Serial.printf("top8: %s bottom2: %s\n", top8, bottom2);
                uint32_t irgbw;
                uint8_t *rgbw = (uint8_t *)&irgbw;
                uint32_t ibrightness;
                uint8_t *brightness = (uint8_t *)&ibrightness;
                sscanf(top8, "%x", &irgbw);
                sscanf(bottom2, "%x", &ibrightness);
                Serial.printf("(Subtract) Got rgbwb: %x %x %x %x %x\n", rgbw[3], rgbw[2], rgbw[1], rgbw[0], brightness[0]);
                ha_light_add_subtract_color(true, rgbw[3], rgbw[2], rgbw[1], rgbw[0], brightness[0]);
            }
            else if (strcmp("red_slider", pActions[i].data.native.command) == 0)
            {
                int targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * 255);
                Serial.printf("Setting red to %x\n",targetValue);
                targetValue = clamp(targetValue, 255, 0);
                ha_light_set_color(true, targetValue, -1, -1, -1, -1);
            }
            else if (strcmp("green_slider", pActions[i].data.native.command) == 0)
            {
                int targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * 255);
                targetValue = clamp(targetValue, 255, 0);
                ha_light_set_color(true, -1, targetValue, -1, -1, -1);
            }
            else if (strcmp("blue_slider", pActions[i].data.native.command) == 0)
            {
                int targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * 255);
                targetValue = clamp(targetValue, 255, 0);
                ha_light_set_color(true, -1, -1, targetValue, -1, -1);
            }
            else if (strcmp("white_slider", pActions[i].data.native.command) == 0)
            {
                int targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * 255);
                targetValue = clamp(targetValue, 255, 0);
                ha_light_set_color(true, -1, -1, -1, targetValue, -1);
            }
            else if (strcmp("brightness_slider", pActions[i].data.native.command) == 0)
            {
                int targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * 255);
                targetValue = clamp(targetValue, 255, 0);
                ha_light_set_brightness(targetValue);
            }
            else if (strcmp("colortemp_slider", pActions[i].data.native.command) == 0)
            {
                const uint16_t bottomRange = 153;
                const uint16_t topRange = 500;
                const double range = topRange - bottomRange;
                uint16_t targetValue = 0;
                if (x > 0)
                    targetValue = ((double(x) / 240) * range) + bottomRange;
                //Serial.printf("colortemp: Percent: %i% topRange: %i bottomRange: %i targetValue: %i\n");
                targetValue = clamp(targetValue, topRange, bottomRange);
                ha_light_set_colortemp(targetValue);
            }
            else
            {
                Serial.printf("Unknown Native Command: %s\n", pActions[i].data.native.command);
            }

            // if (strcmp("Command",...) == 0) ...
            break;
        case ACTION_JUMP:
            
            if (isFirstClick)
            {
              Serial.printf("Running Jump\n");
              jumpToMenu(pActions[i].data.jump.destination);
            }
            break;
        case ACTION_UNSET:
        default:
            Serial.printf("Running nothing (unset)\n");
            break;
        }
    }
}