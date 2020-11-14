#ifndef _MAIN_H
#define _MAIN_H

#include "effect.h"
#include <ESPAsyncWebServer.h>

void UpdateOLED(std::string status);
double FramesPerSecond(double seconds);
bool getParamByte(AsyncWebServerRequest *request, String name, byte &bValue);
bool getParamLong(AsyncWebServerRequest *request, String name, long &lValue);
bool getParamString(AsyncWebServerRequest *request, String name, String &strValue);

template <typename T>
void initializeEffect(Effect effect, String name);

void ConnectToWiFi(const char *ssid, const char *password);
void setup();
void loop();
void setupWebServer();
void handleRouteEffect(AsyncWebServerRequest *request);

#endif