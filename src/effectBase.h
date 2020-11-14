#ifndef _EFFECTBASE_H
#define _EFFECTBASE_H

#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

class EffectBase
{
    protected:
        CRGB* pLEDs;
        int   numLEDs;

        EffectBase(CRGB* pLEDs, int numLEDs) 
        {
            this->pLEDs = pLEDs;
            this->numLEDs = numLEDs;
        }

    public:
        virtual void Draw() = 0;
        virtual void Reset()
        {
            // Default does nothing.
        }
};


#endif
