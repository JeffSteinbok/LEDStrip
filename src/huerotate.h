#ifndef _HUEROTATE_H
#define _HUEROTATE_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

class HueRotate : public EffectBase
{
    public:
        HueRotate(CRGB* pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
        {
        }

        virtual void Draw() 
        {
            const int deltaHue = 1;     // How far to step the cycling hue each draw call.

            static byte hue = HUE_RED;            // Current color
            hue += deltaHue;                      // Update the comet color

            CRGB c;
            c.setHue(hue);
            fill_solid(this->pLEDs, this->numLEDs, c);

            delay(100);
        }
        
};

#endif
