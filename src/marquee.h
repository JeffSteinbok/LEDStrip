#ifndef _MARQUEE_H
#define _MARQUEE_H

#include "effectBase.h"
#include <Arduino.h>
#define FASTLETD_INTERNAL
#include <FastLED.h>


class Marquee : public EffectBase
{
    private:
        byte hueStart;

    public:
        Marquee(CRGB* pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
        {
            this->hueStart = HUE_BLUE;
        }
        
        virtual void Draw() 
        {
            this->hueStart += 4;
            byte k = this->hueStart;

            CRGB c;
            for (int i = 0; i < this->numLEDs; i++)
            {
                this->pLEDs[i] = c.setHue(k+=8);
            }

            static int scroll = 0;
            scroll++;
            
            for (int i = scroll % 5; i < this->numLEDs - 1; i += 5)
            {
                this->pLEDs[i] = CRGB::Black;
            }

            delay(50);
        }
        
};

#endif