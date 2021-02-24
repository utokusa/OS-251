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
}
