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
          level (0.0) {}

    void noteOn();
    void noteOFf();
    void update();
    flnum getLevel() const { return level; }
    void setCurrentPlaybackSampleRate (const double newRate) { sampleRate = newRate; }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    const EnvelopeParams* const p;
    flnum sampleRate;

    State state;
    flnum level;

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
};
} // namespace onsen
