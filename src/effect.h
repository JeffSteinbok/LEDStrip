#ifndef _EFFECT_H
#define _EFFECT_H

enum class Effect : unsigned char {
    Off = 0,
    Solid,
    Marquee,
    Comet,
    HueRotate,
    Twinkle,
};

#define NUM_EFFECTS 6

#endif