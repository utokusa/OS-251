/*
  ==============================================================================

   Oscillator

  ==============================================================================
*/

#pragma once

#include "../synth/SynthParams.h"
#include "DspCommon.h"
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
    flnum oscillatorVal (flnum angleRad, flnum shapeModulationAmount, flnum angleRadInc)
    {
        const flnum firstAngleRad = angleRad;
        const flnum firstAngleRadInc = angleRadInc;
        const flnum secondAngleRad = shapePhase (angleRad * 2, shapeModulationAmount);
        const flnum secondAngleRadInc = angleRadInc * 2.0;

        flnum currentSample = 0.0;
        const auto sinGain = p->getSinGain();
        const auto squareGain = p->getSquareGain();
        const auto sawGain = p->getSawGain();
        const auto subSquareGain = p->getSubSquareGain();
        const auto noiseGain = p->getNoiseGain();

        if (sinGain > 0.0)
            currentSample += sinWave (secondAngleRad) * sinGain;
        if (squareGain > 0.0)
            currentSample += squareWave (secondAngleRad, secondAngleRadInc) * squareGain;
        if (sawGain > 0.0)
            currentSample += sawWave (secondAngleRad, secondAngleRadInc) * sawGain;
        if (subSquareGain > 0.0)
            currentSample += squareWave (firstAngleRad, firstAngleRadInc) * subSquareGain;
        if (noiseGain > 0.0)
            currentSample += noiseWave() * noiseGain;

        return currentSample;
    }

    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        smoothedShape.prepareToPlay (sampleRate);
    }

    void resetState()
    {
        smoothedShape.reset (0.0);
    }

private:
    static constexpr bool ANTI_ALIAS = false;
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

    flnum polyBlep (flnum angle, flnum angleInc)
    {
        flnum dt = angleInc / (2.0 * pi);
        flnum t = angle / (2.0 * pi);
        if (t < dt)
        {
            t /= dt;
            return -t * t + 2.0 * t - 1.0;
        }
        else if (t > 1.0 - dt)
        {
            t = (t - 1.0) / dt;
            return t * t + 2.0 * t + 1.0;
        }
        else
        {
            return 0.0;
        }
    }

    // TODO: extract waveforms as function

    static flnum sinWave (flnum angle)
    {
        return std::sin (angle);
    }

    static flnum squareWaveNaive (flnum angle)
    {
        return angle < pi ? 1.0 : -1.0;
    }

    flnum squareWave (flnum angle, flnum angleInc)
    {
        if (ANTI_ALIAS)
        {
            flnum val = squareWaveNaive (angle);
            val += polyBlep (angle, angleInc);
            val -= polyBlep (fmod (angle + pi, 2.0 * pi), angleInc);
            return val;
        }
        else
        {
            return squareWaveNaive (angle);
        }
    }

    flnum sawWave (flnum angle, flnum angleInc)
    {
        if (ANTI_ALIAS)
        {
            flnum val = sawWaveNaive (angle);
            val -= polyBlep (angle, angleInc);
            return val;
        }
        else
        {
            return sawWaveNaive (angle);
        }
    }

    static flnum sawWaveNaive (flnum angle)
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
        flnum shaped = 2.0 * pi * (shape * curve (normalizedAngle) + (1.0 - shape) * normalizedAngle);
        return shaped;
    }

    // Taeks [0,1] and returns [0,1]
    flnum curve (flnum in0to1)
    {
        flnum out0to1 = in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1 * in0to1;
        return out0to1;
    }
};
} // namespace onsen
