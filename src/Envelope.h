/*
  ==============================================================================

   Envelope

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"

namespace onsen
{
//==============================================================================
class Envelope
{
    enum class State
    {
        OFF,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

public:
    Envelope() = delete;
    Envelope (SynthParams* const synthParams)
        : p (synthParams->envelope()),
          sampleRate (DEFAULT_SAMPLE_RATE),
          state (State::OFF),
          level (0.0),
          noteOffLevel(0.0),
          sampleCnt(0) {}

    void noteOn();
    void noteOFf();
    void update();
    flnum getLevel() const { return level; }
    flnum isEnvOff() const {return state == State::RELEASE && level < 0.005; }
    void setCurrentPlaybackSampleRate (const double newRate) { sampleRate = newRate; }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    const EnvelopeParams* const p;
    flnum sampleRate;

    State state;
    flnum level;
    flnum noteOffLevel;
    int sampleCnt;

    // Adjust parameter value like attack, decay or release according to the
    // sampling rate
    flnum adjust (const flnum val) const
    {
        // If no need to adjust
        if (std::abs (sampleRate - DEFAULT_SAMPLE_RATE) <= EPSILON)
        {
            return val;
        }
        const flnum amount = std::pow (val, DEFAULT_SAMPLE_RATE / sampleRate - 1);
        return val * amount;
    }

    int toSample(flnum timeSec)
    {
        return timeSec * sampleRate;
    }

    flnum toTimeSec(int sample)
    {
        return sample / sampleRate; 
    }

    // Return value [0, 1]
    flnum attackCurve(flnum curTimeSec, flnum lengthSec)
    {
        return curTimeSec / lengthSec;
    }

    // Return value [0, 1]
    flnum decayCurve(flnum curTimeSec, flnum lengthSec)
    {
        return (lengthSec - curTimeSec) / lengthSec;
    }

    // Return value [0, 1]
    flnum releaseCurve(flnum curTimeSec, flnum lengthSec)
    {
        return  (lengthSec - curTimeSec) / lengthSec;
    }
};
} // namespace onsen
