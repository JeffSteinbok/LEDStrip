#ifndef _HUEROTATE_H
#define _HUEROTATE_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

class HueRotate : public EffectBase
{
public:
    HueRotate(CRGB *pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
    {
    }

    virtual bool Draw()
    {
        bool bRet = false;

        TIMES_PER_SECOND(10)
        {
            bRet = true;

            const int deltaHue = 1; // How far to step the cycling hue each draw call.

            static byte hue = HUE_RED; // Current color
            hue += deltaHue;           // Update the comet color

            CRGB c;
            c.setHue(hue);
            fill_solid(this->pLEDs, this->numLEDs, c);
        }

        return bRet;
    }
};

#endif
