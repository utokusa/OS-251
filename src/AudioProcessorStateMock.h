/*
  ==============================================================================

   AudioProcessorStateMock

  ==============================================================================
*/

#pragma once

#include "IAudioProcessorState.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class AudioProcessorStateMock : public IAudioProcessorState
{
public:
    AudioProcessorStateMock()
        : state (juce::Identifier ("OS-251"))
    {
    }

    juce::String getProcessorName() override
    {
        return state.getType().toString();
    }

    void replaceState (const juce::ValueTree& newState) override
    {
        state = newState;
    }

    juce::ValueTree copyState() override
    {
        return state;
    }

    juce::ValueTree* getState() override
    {
        return &state;
    }

    void setPreset (juce::String relativePresetPath) override
    {
        AudioProcessorStateUtil::setPreset (state, relativePresetPath);
    }

    /*
    Returns relative preset path
    */
    juce::String getPreset() override
    {
        return AudioProcessorStateUtil::getPreset (state);
    }

    //==============================================================================
private:
    juce::ValueTree state;
};
} // namespace onsen
