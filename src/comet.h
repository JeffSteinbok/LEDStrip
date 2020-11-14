#ifndef _COMET_H
#define _COMET_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

struct CometConfig 
{
    byte length = 10;
};

class Comet : public EffectBase
{
    private:
        byte hue;        // Current color
        int iDirection;  // Current direction (-1 or +1)
        double iPos;     // Current comet position

        CometConfig config;
        
    public:
        Comet(CRGB* pLEDs, int numLEDs);
        void SetConfig(CometConfig& config);
        virtual void Draw();
        virtual void Reset();
};


#endif
