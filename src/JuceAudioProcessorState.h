/*
  ==============================================================================

   JuceAudioProcessorState

  ==============================================================================
*/

#pragma once

#include "IAudioProcessorState.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class JuceAudioProcessorState : public IAudioProcessorState
{
public:
    JuceAudioProcessorState (juce::AudioProcessorValueTreeState& _processorState)
        : processorState (_processorState)
    {
    }

    juce::String getProcessorName() override
    {
        return processorState.state.getType().toString();
    }

    void replaceState (const juce::ValueTree& newState) override
    {
        processorState.replaceState (newState);
    }

    juce::ValueTree copyState() override
    {
        return processorState.copyState();
    }

    //==============================================================================
private:
    juce::AudioProcessorValueTreeState& processorState;
};
} // namespace onsen
