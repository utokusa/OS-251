/*
  ==============================================================================

   Oscillator

  ==============================================================================
*/

#pragma once

#include <random>
#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"

namespace onsen
{
//==============================================================================
class Oscillator
{
    using flnum = float;

public:
    Oscillator() = delete;
    Oscillator (SynthParams* const synthParams)
        : p (synthParams->oscillator()),
          randEngine (seedGen()),
          randDist (0.0, 1.0)
    {
    }

    // Return oscillator voltage value.
    // Angle is in radian.
    flnum oscillatorVal (flnum angle)
    {
        flnum currentSample = 0.0;
        currentSample += sinWave (wrapAngle (angle * 2)) * p->getSinGain();
        currentSample += squareWave (wrapAngle (angle * 2)) * p->getSquareGain();
        currentSample += sawWave (wrapAngle (angle * 2)) * p->getSawGain();
        currentSample += squareWave (angle) * p->getSubSquareGain();
        currentSample += noiseWave() * p->getNoiseGain();
        return currentSample;
    }

private:
    const OscillatorParams* const p;
    std::random_device seedGen;
    std::default_random_engine randEngine;
    std::uniform_real_distribution<> randDist;

    static flnum wrapAngle (flnum angle)
    {
        while (angle > 2 * pi)
        {
            angle -= 2 * pi;
        }
        return angle;
    }

    // TODO: extract waveforms as function

    static flnum sinWave (flnum angle)
    {
        return std::sin (angle);
    }

    static flnum squareWave (flnum angle)
    {
        return angle < pi ? 1.0 : -1.0;
    }

    static flnum sawWave (flnum angle)
    {
        return std::min (2.0 * angle / (2.0 * pi), 2.0) - 1.0;
    }

    flnum noiseWave()
    {
        return randDist (randEngine);
    }
};
}
