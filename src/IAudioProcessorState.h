/*
  ==============================================================================

   IAudioProcessorState

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class IAudioProcessorState
{
public:
    virtual void replaceState (const juce::ValueTree& newState) = 0;
    virtual juce::String getProcessorName() = 0;
    virtual juce::ValueTree copyState() = 0;
};
} // namespace onsen
