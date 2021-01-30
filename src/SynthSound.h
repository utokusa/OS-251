/*
  ==============================================================================

   OS-251 synthesizer's sound

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"

namespace onsen
{
//==============================================================================
struct FancySynthSound : public juce::SynthesiserSound
{
    FancySynthSound() = default;

    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};
}
