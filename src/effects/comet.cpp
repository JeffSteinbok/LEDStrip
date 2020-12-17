#include "comet.h"

Comet::Comet(CRGB *pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
{
    hue = HUE_RED; // Current color
    Reset();
}

void Comet::Reset()
{
    iDirection = 1; // Current direction (-1 or +1)
    dPos = 0.0;     // Current comet position
    this->cometSpeed = (double)this->config.speed;
}

void Comet::SetConfig(CometConfig &config)
{
    this->config.length = config.length;
    this->config.speed = config.speed;
    this->Reset();
}

bool Comet::Draw()
{
    bool bRet = false;

    TIMES_PER_SECOND(30)
    {
        bRet = true;

        const byte fadeAmt = 64; // Fraction of 256 to fade
        const int deltaHue = 4;  // How far to step the cycling hue each draw call.

        hue += deltaHue;                       // Update the comet color
        dPos += iDirection * this->cometSpeed; // Update the position

        int maxPos = numLEDs - this->config.length;

        // It's possible here that the position can be negative or beyond the range of
        // the LEDs, so for safety, we'll bound it.
        if (dPos < 0)
            dPos = 0;
        if (dPos > maxPos)
            dPos = maxPos;

        // Flip the comet direction when it hits the end.
        if (dPos == maxPos || dPos == 0)
        {
            iDirection *= -1;
        }

        // Draw the comet at its current position
        for (int i = 0; i < this->config.length; i++)
        {
            pLEDs[(int)dPos + i].setHue(hue);
        }

        // Fade the LEDs one step
        for (int j = 0; j < numLEDs; j++)
        {
            if (random(2) == 1)
            {
                pLEDs[j] = pLEDs[j].fadeToBlackBy(fadeAmt);
            }
        }
    }
    return bRet;
}
