#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

#define NUM_COLORS 5
static const CRGB TwinkleColors[NUM_COLORS] =
    {
        CRGB::Red,
        CRGB::Blue,
        CRGB::Purple,
        CRGB::Green,
        CRGB::Orange};

#ifndef _TWINKLE_H
#define _TWINKLE_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

class Twinkle : public EffectBase
{
public:
    Twinkle(CRGB *pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
    {
    }

    virtual void Reset()
    {
        FastLED.clear(false);
    }

    virtual bool Draw()
    {
        TIMES_PER_SECOND(5)
        {
            static int passCount = 0;

            // We reset the strip after 1/4 of the LEDs are lit
            if (passCount++ == this->numLEDs / 4)
            {
                passCount = 0;
                FastLED.clear(false);
            }
            this->pLEDs[random(this->numLEDs)] = TwinkleColors[random(0, NUM_COLORS)];

            return true;
        }

        return false;
    }
};

#endif
