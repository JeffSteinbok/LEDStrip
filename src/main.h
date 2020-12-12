#ifndef _MAIN_H
#define _MAIN_H

#include "effect.h"
#include <ESPAsyncWebServer.h>

void UpdateOLED(const String &status);
double FramesPerSecond(double seconds);
bool getParamByte(AsyncWebServerRequest *request, const String &name, byte &bValue);
bool getParamLong(AsyncWebServerRequest *request, const String &name, long &lValue);
bool getParamString(AsyncWebServerRequest *request, const String &name, String &strValue);

template <typename T>
void initializeEffect(Effect effect, String name);

void ConnectToWiFi(const char *ssid, const char *password);
void setup();
void loop();
void setupWebServer();
void handleRouteEffect(AsyncWebServerRequest *request);

#endif
