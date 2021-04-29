#include <Arduino.h>
#include <U8g2lib.h>
#define FASTLED_INTERNAL
#include <FastLED.h>
#include <ESPAsyncWebServer.h>
#include <FS.h>
#include <SPIFFS.h>
#include "WiFi.h"       // ESP32 WiFi include
#include "WiFiConfig.h" // My WiFi configuration.
#include "main.h"
#include "Update.h"

#define OLED_CLOCK 15 // Pins for the OLED display
#define OLED_DATA 4
#define OLED_RESET 16

#define NUM_LEDS 144 // FastLED definitions
#define LED_PIN 5

CRGB g_LEDs[NUM_LEDS] = {0}; // Frame buffer for FastLED
int g_numLEDs = NUM_LEDS;

U8G2_SSD1306_128X64_NONAME_F_HW_I2C g_OLED(U8G2_R2, OLED_RESET, OLED_CLOCK, OLED_DATA);
int g_lineHeight = 0;
int g_Brightness = 5; // 0-255 brightness scale
int g_powerLimit = 900;
int g_cometLength = 10;

AsyncWebServer server(80); // Object of WebServer(HTTP port, 80 is defult)

#include "effect.h"
#include "effectBase.h"
#include "solidEffect.h"
#include "marquee.h"
#include "twinkle.h"
#include "effects/comet.h"
#include "effects/bouncingBallEffect.h"
#include "huerotate.h"
#include "ArduinoOTA.h"

bool g_fIsOn = true;
Effect g_runningEffect = Effect::HueRotate;
EffectBase *g_effects[NUM_EFFECTS];
std::map<String, Effect> effectMap;

String g_OLEDMessage = "";
String g_indexJSCache = "";

void UpdateOLED(const String &status)
{
    g_OLED.clearBuffer();
    g_OLED.setCursor(0, g_lineHeight);

    // This is REALLY ugly.  I don't like it at all.  Assumes string is null terminated.
    // Does seem to work in this case though.
    g_OLED.print(status);

    g_OLED.sendBuffer();
}

bool getParamByte(AsyncWebServerRequest *request, const String &name, byte &bValue)
{
    if (request->hasParam(name))
    {
        AsyncWebParameter *p = request->getParam(name);
        Serial.printf("Found paramter [%s]: %s\n", name, p->value());
        bValue = static_cast<byte>(p->value().toInt());
        return true;
    }
    return false;
}

bool getParamLong(AsyncWebServerRequest *request, const String &name, long &lValue)
{
    if (request->hasParam(name))
    {
        AsyncWebParameter *p = request->getParam(name);
        Serial.printf("Found paramter [%s]: %s\n", name, p->value());
        lValue = strtol(p->value().c_str(), NULL, 16);
        return true;
    }
    return false;
}

bool getParamString(AsyncWebServerRequest *request, const String &name, String &strValue)
{
    if (request->hasParam(name))
    {
        AsyncWebParameter *p = request->getParam(name);
        Serial.printf("Found paramter [%s]: %s\n", name, p->value());
        strValue = p->value();
        return true;
    }
    return false;
}

template <typename T>
void initializeEffect(Effect effect, String name)
{
    effectMap[name] = effect;
    g_effects[static_cast<int>(effect)] = new T(g_LEDs, g_numLEDs);
}

/*
 * ConnectToWifi
 */
void ConnectToWiFi(const char *ssid, const char *password)
{

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to ");
    Serial.println(SSID);
    UpdateOLED("Connecting to Wifi...");

    uint8_t i = 0;
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(500);

        if ((++i % 16) == 0)
        {
            Serial.println(F(" still trying to connect"));
            UpdateOLED("Still Connecting to Wifi...");
        }
    }

    Serial.print(F("Connected. My IP address is: "));
    Serial.println(WiFi.localIP());
}

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(LED_PIN, OUTPUT);

    Serial.begin(115200);
    while (!Serial)
    {
    }

    Serial.println("ESP32 Startup");

    g_OLED.begin();
    g_OLED.clear();
    g_OLED.setFont(u8g2_font_profont15_tf);
    g_lineHeight = g_OLED.getFontAscent() - g_OLED.getFontDescent(); // Descent is a negative number so we add it to the total

    ConnectToWiFi(SSID, WiFiPassword);

    // Port defaults to 3232
    ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    ArduinoOTA
        .onStart([]() {
            String type;
            if (ArduinoOTA.getCommand() == U_FLASH)
                type = "sketch";
            else // U_SPIFFS
                type = "filesystem";

            // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
            Serial.println("Start updating " + type);
        })
        .onEnd([]() {
            Serial.println("\nEnd");
        })
        .onProgress([](unsigned int progress, unsigned int total) {
            Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
        })
        .onError([](ota_error_t error) {
            Serial.printf("Error[%u]: ", error);
            if (error == OTA_AUTH_ERROR)
                Serial.println("Auth Failed");
            else if (error == OTA_BEGIN_ERROR)
                Serial.println("Begin Failed");
            else if (error == OTA_CONNECT_ERROR)
                Serial.println("Connect Failed");
            else if (error == OTA_RECEIVE_ERROR)
                Serial.println("Receive Failed");
            else if (error == OTA_END_ERROR)
                Serial.println("End Failed");
        });

    ArduinoOTA.begin();

    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // Initialize SPIFFS

    if (!SPIFFS.begin())
    {
        Serial.println("An Error has occurred while mounting LittleFS");
        return;
    }

    setupWebServer();

    FastLED.addLeds<WS2812B, LED_PIN, GRB>(g_LEDs, NUM_LEDS); // Add our LED strip to the FastLED library
    FastLED.setBrightness(g_Brightness);

    initializeEffect<SolidEffect>(Effect::Solid, "solid");
    initializeEffect<Marquee>(Effect::Marquee, "marquee");
    initializeEffect<Comet>(Effect::Comet, "comet");
    initializeEffect<HueRotate>(Effect::HueRotate, "huerotate");
    initializeEffect<Twinkle>(Effect::Twinkle, "twinkle");
    initializeEffect<BouncingBallEffect>(Effect::BouncingBall, "bouncingBall");

    // Start server
    server.begin();
}

void loop()
{

    ArduinoOTA.handle();

    if (g_fIsOn)
    {
        if (g_effects[static_cast<int>(g_runningEffect)]->Draw())
        {
            FastLED.setBrightness(g_Brightness);
            FastLED.setMaxPowerInMilliWatts(g_powerLimit); // Not sure this is the value I want to use, but ok for now.
            set_max_power_indicator_LED(LED_BUILTIN);

            FastLED.show();
        }

        // Handle OLED drawing
        EVERY_N_MILLISECONDS(250)
        {
            //uint32_t milliwatts = calculate_unscaled_power_mW(g_LEDs, g_numLEDs);
            int lineNum = 1;

            g_OLED.clearBuffer();
            g_OLED.setCursor(0, g_lineHeight * lineNum++);
            g_OLED.printf("IP: ");

            // This is REALLY ugly.  I don't like it at all.  Assumes string is null terminated.
            // Does seem to work in this case though.
            g_OLED.print(WiFi.localIP().toString());

            g_OLED.setCursor(0, g_lineHeight * lineNum++);
            g_OLED.printf("FPS: %u", FastLED.getFPS());

            //g_OLED.setCursor(0, g_lineHeight * lineNum++);
            //g_OLED.printf("Power: %u mW", milliwatts);

            g_OLED.setCursor(0, g_lineHeight * lineNum++);
            g_OLED.print(g_OLEDMessage);

            g_OLED.setCursor(0, g_lineHeight * lineNum++);
            g_OLED.printf("Brite: %d", calculate_max_brightness_for_power_mW(g_Brightness, g_powerLimit));

            g_OLED.sendBuffer();
        }
    }
    else
    {

        fill_solid(g_LEDs, NUM_LEDS, CRGB::Black);
        delay(50);
        FastLED.setBrightness(0);
        FastLED.show();
    }
}

void setupWebServer()
{
    // Route to load index.html file
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    // Cache the JS file as it's the only significant content and loading from
    // flash every time causes the LEDs to flicker.  RAM is faster.
    File file = SPIFFS.open("/index.js");
    g_indexJSCache += file.readString();
    server.on("/index.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        //request->send(SPIFFS, "/index.js", "text/javascript");
        request->send(200, "text/javascript", g_indexJSCache);
    });

    // Route to load led.css file
    server.on("/index.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.css", "text/css");
    });

    server.on("/effect", HTTP_GET, [](AsyncWebServerRequest *request) {
        handleRouteEffect(request);
    });

    server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
        // Handling function implementation
        g_fIsOn = false;
        request->send(200, "text/html", "Yo!");
    });
}

void handleRouteEffect(AsyncWebServerRequest *request)
{
    g_fIsOn = true;

    if (request->hasParam("brightness"))
    {
        AsyncWebParameter *p = request->getParam("brightness");
        g_Brightness = p->value().toInt();
    }

    // Get the effect Paramter
    if (request->hasParam("name"))
    {
        AsyncWebParameter *p = request->getParam("name");
        Serial.printf("Found [name] paramter: %s\n", p->value());

        if (effectMap.count(p->value()))
        {
            g_runningEffect = effectMap[p->value()];
            g_OLEDMessage = "Change to: " + p->value();

            switch (g_runningEffect)
            {

            case Effect::Solid:
            {
                SolidEffectConfig solidConfig;
                long lColor;
                if (getParamLong(request, "color", lColor))
                {
                    solidConfig.color = lColor;
                }
                static_cast<SolidEffect *>(g_effects[static_cast<int>(Effect::Solid)])->SetConfig(solidConfig);
                break;
            }
            case Effect::Marquee:
            {
                break;
            }
            case Effect::Comet:
            {
                CometConfig cometConfig;
                getParamByte(request, "length", cometConfig.length);
                getParamByte(request, "speed", cometConfig.speed);
                static_cast<Comet *>(g_effects[static_cast<int>(Effect::Comet)])->SetConfig(cometConfig);
                break;
            }
            case Effect::Twinkle:
            {
                static_cast<Twinkle *>(g_effects[static_cast<int>(Effect::Twinkle)])->Reset();
                break;
            }
            case Effect::BouncingBall:
            {
                BouncingBallEffectConfig bouncingBallEffectConfig;
                getParamByte(request, "ballCount", bouncingBallEffectConfig.ballCount);
                getParamByte(request, "fade", bouncingBallEffectConfig.fade);
                static_cast<BouncingBallEffect *>(g_effects[static_cast<int>(Effect::BouncingBall)])->SetConfig(bouncingBallEffectConfig);
                break;
            }
            }
        }
        else
        {
            Serial.print("No matching effect.");
        }
    }

    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", "Ok");
    response->addHeader("Access-Control-Allow-Origin", "*");
    request->send(response);
}
