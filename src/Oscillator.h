/*
  ==============================================================================

   Oscillator

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "SynthParams.h"
#include <JuceHeader.h>
#include <random>

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
    flnum oscillatorVal (flnum angle, flnum shapeModulationAmount)
    {
        const flnum firstAngle = angle;
        const flnum secondAngle = shapePhase (angle * 2, shapeModulationAmount);

        flnum currentSample = 0.0;
        currentSample += sinWave (secondAngle) * p->getSinGain();
        currentSample += squareWave (secondAngle) * p->getSquareGain();
        currentSample += sawWave (secondAngle) * p->getSawGain();
        currentSample += squareWave (firstAngle) * p->getSubSquareGain();
        currentSample += noiseWave() * p->getNoiseGain();

        return currentSample;
    }

private:
    OscillatorParams* const p;
    std::random_device seedGen;
    std::default_random_engine randEngine;
    std::uniform_real_distribution<> randDist;

    static flnum wrapAngle (flnum angle)
    {
        while (angle > 2.0 * pi)
        {
            angle -= 2.0 * pi;
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

    flnum shapePhase (flnum angle, flnum shapeModulationAmount)
    {
        angle = wrapAngle (angle);
        flnum shape = std::clamp<flnum> (p->getShape() + shapeModulationAmount, 0.0, 1.0);
        flnum normalizedAngle = std::clamp (angle / (2.0 * pi), 0.0, 1.0);
        flnum shaped = 2.0 * pi * (shape * map (normalizedAngle) + (1.0 - shape) * normalizedAngle);
        return shaped;
    }

    flnum map (flnum in0to1)
    {
        flnum out0to1 = in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1;
        return out0to1;
    }
};
} // namespace onsen
