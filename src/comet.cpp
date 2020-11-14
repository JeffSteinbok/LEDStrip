#include "comet.h"

Comet::Comet(CRGB* pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
{
    hue = HUE_RED;  // Current color
    Reset();
}

void Comet::Reset() {
    iDirection = 1;  // Current direction (-1 or +1)
    iPos = 0.0;   // Current comet position
}

void Comet::SetConfig(CometConfig& config)
{
    this->config.length = config.length;
    this->Reset();
}

void Comet::Draw()
{
    const byte fadeAmt = 64;    // Fraction of 256 to fade
    const int deltaHue = 4;     // How far to step the cycling hue each draw call.
    const double cometSpeed = 0.5; // How far to advance the comet each frame
    

    hue += deltaHue;                      // Update the comet color
    iPos += iDirection * cometSpeed;      // Update the position

    // Flip the comet direction when it hits the end
    if (iPos == (numLEDs - this->config.length) || iPos == 0)
    {
        iDirection *= -1;
    }

    // Draw the comet at its current position
    for (int i = 0; i < this->config.length; i++)
    {
        pLEDs[(int)iPos + i].setHue(hue);
    }

    // Fade the LEDs one step
    for (int j = 0; j < numLEDs; j++)
    {
        if (random(2) == 1)
        {
            pLEDs[j] = pLEDs[j].fadeToBlackBy(fadeAmt);
        }
    }

    delay(10);
}