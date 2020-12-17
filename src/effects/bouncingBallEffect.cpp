#include "bouncingBallEffect.h"

#include <sys/time.h> // For time-of-day

#include <Arduino.h>
#define FASTLED_INTERNAL
#include <FastLED.h>

using namespace std;
#include <vector>

#define ARRAYSIZE(x) (sizeof(x) / sizeof(x[0])) // Count elements in a static array

vector<double> ClockTimeAtLastBounce;
vector<double> Height;
vector<double> BallSpeed;
vector<double> Dampening;
vector<CRGB> Colors;

static const CRGB ballColors[] =
    {
        CRGB::Green,
        CRGB::Red,
        CRGB::Blue,
        CRGB::Orange,
        CRGB::Indigo};

BouncingBallEffect::BouncingBallEffect(CRGB *pLEDs, int numLEDs) : EffectBase(pLEDs, numLEDs)
{
    Reset();
}

double BouncingBallEffect::Time()
{
    timeval tv = {0};
    gettimeofday(&tv, nullptr);
    return (double)(tv.tv_usec / 1000000.0 + (double)tv.tv_sec);
}

double BouncingBallEffect::InitialBallSpeed(double height) const
{
    return sqrt(-2 * Gravity * height); // Because MATH!
}

void BouncingBallEffect::Reset()
{
    ClockTimeAtLastBounce.resize(this->config.ballCount);
    Height.resize(this->config.ballCount);
    BallSpeed.resize(this->config.ballCount);
    Dampening.resize(this->config.ballCount);
    Colors.resize(this->config.ballCount);

    for (size_t i = 0; i < this->config.ballCount; i++)
    {
        Height[i] = StartHeight;                                  // Current Ball Height
        ClockTimeAtLastBounce[i] = Time();                        // When ball last hit ground state
        Dampening[i] = 0.90 - i / pow(this->config.ballCount, 2); // Bounciness of this ball
        BallSpeed[i] = InitialBallSpeed(Height[i]);               // Don't dampen initial launch
        Colors[i] = ballColors[i % ARRAYSIZE(ballColors)];
    }

    Serial.printf("Bouncing Ball Reset");
    Serial.printf("BallCount: %i\n", this->config.ballCount);
    Serial.printf("Mirrored: %i\n", this->config.bMirrored);
    Serial.printf("Fade: %i\n", this->config.fade);
}

void BouncingBallEffect::SetConfig(BouncingBallEffectConfig &config)
{
    this->config.ballCount = config.ballCount;
    this->config.bMirrored = config.bMirrored;
    this->config.fade = config.fade;
    this->Reset();
}

bool BouncingBallEffect::Draw()
{
    bool bRet = false;

    TIMES_PER_SECOND(30)
    {
        bRet = true;

        if (this->config.fade != 0)
        {
            for (size_t i = 0; i < this->numLEDs; i++)
                this->pLEDs[i].fadeToBlackBy(this->config.fade);
        }
        else
        {
            FastLED.clear();
        }

        // Draw each of the balls

        for (size_t i = 0; i < this->config.ballCount; i++)
        {
            double TimeSinceLastBounce = (Time() - ClockTimeAtLastBounce[i]) / SpeedKnob;

            // Use standard constant acceleration function - https://en.wikipedia.org/wiki/Acceleration
            Height[i] = 0.5 * Gravity * pow(TimeSinceLastBounce, 2.0) + BallSpeed[i] * TimeSinceLastBounce;

            // Ball hits ground - bounce!
            if (Height[i] < 0)
            {
                Height[i] = 0;
                BallSpeed[i] = Dampening[i] * BallSpeed[i];
                ClockTimeAtLastBounce[i] = Time();

                if (BallSpeed[i] < 0.01)
                    BallSpeed[i] = InitialBallSpeed(StartHeight) * Dampening[i];
            }

            size_t position = (size_t)(Height[i] * (this->numLEDs - 1) / StartHeight);

            this->pLEDs[position] += Colors[i];
            this->pLEDs[position + 1] += Colors[i];

            if (this->config.bMirrored)
            {
                this->pLEDs[this->numLEDs - 1 - position] += Colors[i];
                this->pLEDs[this->numLEDs - position] += Colors[i];
            }
        }

        return bRet;
    }
}
