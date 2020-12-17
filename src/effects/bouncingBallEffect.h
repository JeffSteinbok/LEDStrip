#ifndef _BOUNCINGBALLEFFECT_H
#define _BOUNCINGBALLEFFECT_H

#include "effectBase.h"
#define FASTLETD_INTERNAL
#include <FastLED.h>

using namespace std;
#include <vector>

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0])) // Count elements in a static array

struct BouncingBallEffectConfig
{
    byte ballCount = 3;
    byte fade = 0;
    bool bMirrored = false;
};

class BouncingBallEffect : public EffectBase
{
private:
    double InitialBallSpeed(double height) const;

    const double Gravity = -9.81; // Because PHYSICS!
    const double StartHeight = 1; // Drop balls from max height initially
    const double ImpactVelocity = InitialBallSpeed(StartHeight);
    const double SpeedKnob = 4.0; // Higher values will slow the effect

    static double Time();
    BouncingBallEffectConfig config;

public:
    BouncingBallEffect(CRGB *pLEDs, int numLEDs);
    void SetConfig(BouncingBallEffectConfig &config);
    virtual bool Draw();
    virtual void Reset();
};

#endif
