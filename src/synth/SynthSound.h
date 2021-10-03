/*
  ==============================================================================

   OS-251 synthesizer's sound

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
struct FancySynthSound : public juce::SynthesiserSound
{
    FancySynthSound() = default;

    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};
} // namespace onsen
