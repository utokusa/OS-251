/*
  ==============================================================================

   DSP common

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
using flnum = float;
static constexpr flnum DEFAULT_SAMPLE_RATE = 44100.0;
static constexpr int DEFAULT_SAMPLES_PER_BLOCK = 512;
static constexpr flnum pi = juce::MathConstants<flnum>::pi;
static constexpr flnum EPSILON = std::numeric_limits<flnum>::epsilon();
//==============================================================================
class DspUtil
{
public:
    // Map value [0.0, 1.0] to [-1.0, 1.0].
    static flnum valMinusOneToOne (flnum valZeroToOne)
    {
        return std::clamp<flnum> ((valZeroToOne - 0.5) * 2.0, -1.0, 1.0);
    }
};

class SmoothFlnum
{
public:
    SmoothFlnum (flnum val, flnum _smoothness)
        : sampleRate (DEFAULT_SAMPLE_RATE),
          target (val),
          cur (val),
          smoothness (_smoothness),
          adjustedSmoothness(_smoothness) {}
    flnum get() { return cur = adjustedSmoothness * cur + (1 - adjustedSmoothness) * target; }
    void set (flnum val) { target = val; }
    void prepareToPlay (double _sampleRate)
    {
        sampleRate = _sampleRate;
        adjustedSmoothness = adjust(smoothness);
    }

private:
    flnum sampleRate;
    flnum target;
    flnum cur;
    flnum smoothness; // TODO: Adjust when sample rate changes
    flnum adjustedSmoothness;

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
