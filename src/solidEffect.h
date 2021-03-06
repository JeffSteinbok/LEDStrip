#ifndef _SOLID_H
#define _SOLID_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>

struct SolidEffectConfig
{
    CRGB color;
};

class SolidEffect : public EffectBase
{
private:
    SolidEffectConfig config;

public:
    SolidEffect(CRGB *pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
    {
        this->config.color.setRGB(255, 0, 0);
    }

    void SetConfig(SolidEffectConfig &config)
    {
        this->config.color = config.color;
        this->Reset();
    }

    virtual bool Draw()
    {
        bool bRet = false;
        TIMES_PER_SECOND(10)
        {
            bRet = true;
            fill_solid(this->pLEDs, this->numLEDs, this->config.color);
        }

        return bRet;
    }
};

#endif
