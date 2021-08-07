/*
  ==============================================================================

   Oscillator

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "SynthParams.h"
#include <random>

namespace onsen
{
//==============================================================================
class Oscillator
{
    using flnum = float;

public:
    Oscillator() = delete;
    Oscillator (IOscillatorParams* const oscillatorParams)
        : p (oscillatorParams),
          randEngine (seedGen()),
          randDist (0.0, 1.0),
          smoothedShape (0.0, 0.995)
    {
    }

    // Return oscillator voltage value.
    // Angle is in radian.
    flnum oscillatorVal (flnum angleRad, flnum shapeModulationAmount)
    {
        const flnum firstAngleRad = angleRad;
        const flnum secondAngleRad = shapePhase (angleRad * 2, shapeModulationAmount);

        flnum currentSample = 0.0;
        currentSample += sinWave (secondAngleRad) * p->getSinGain();
        currentSample += squareWave (secondAngleRad) * p->getSquareGain();
        currentSample += sawWave (secondAngleRad) * p->getSawGain();
        currentSample += squareWave (firstAngleRad) * p->getSubSquareGain();
        currentSample += noiseWave() * p->getNoiseGain();

        return currentSample;
    }

    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        smoothedShape.prepareToPlay (sampleRate);
    }

private:
    IOscillatorParams* const p;
    std::random_device seedGen;
    std::default_random_engine randEngine;
    std::uniform_real_distribution<> randDist;
    SmoothFlnum smoothedShape;

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
        smoothedShape.set (p->getShape() + shapeModulationAmount);
        smoothedShape.update();
        flnum shape = std::clamp<flnum> (smoothedShape.get(), 0.0, 1.0);
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
