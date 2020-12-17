#ifndef _COMET_H
#define _COMET_H

#include "effectBase.h"
#define FASTLETD_INTERNAL
#include <FastLED.h>

struct CometConfig
{
    byte length = 10;
    byte speed = 1;
};

class Comet : public EffectBase
{
private:
    byte hue;       // Current color
    int iDirection; // Current direction (-1 or +1)
    double dPos;    // Current comet position

    CometConfig config;
    double cometSpeed; // Actual speed

public:
    Comet(CRGB *pLEDs, int numLEDs);
    void SetConfig(CometConfig &config);
    virtual bool Draw();
    virtual void Reset();
};

#endif
