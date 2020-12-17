#ifndef _EFFECTBASE_H
#define _EFFECTBASE_H

#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

#define TIMES_PER_SECOND(x) EVERY_N_MILLISECONDS(1000 / x)
#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0]))

class EffectBase
{
protected:
    CRGB *pLEDs;
    int numLEDs;

    EffectBase(CRGB *pLEDs, int numLEDs)
    {
        this->pLEDs = pLEDs;
        this->numLEDs = numLEDs;
    }

public:
    virtual bool Draw() = 0;
    virtual void Reset()
    {
        // Default does nothing.
    }
};

#endif
