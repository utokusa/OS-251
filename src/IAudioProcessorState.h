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
    virtual juce::ValueTree* getState() = 0;
    virtual void setPresetName (juce::String relativePresetPath) = 0;
    virtual juce::String getPreset() = 0;
};

namespace AudioProcessorStateUtil
{
    inline void setPresetName (juce::ValueTree& state, juce::String relativePresetPath)
    {
        auto cp = state.getChildWithName (juce::Identifier ("CurrentPreset"));

        if (! cp.isValid() /*For most case, "invalid" means CurrentPreset doesn't exist*/)
        {
            // This block is for processor state without CurrentPreset.
            // It happens when the state is created by older version of OS-251 (before v1.1.0).
            juce::ValueTree preset (juce::Identifier ("CurrentPreset"));
            preset.setProperty (juce::Identifier ("path"), relativePresetPath, nullptr);
            state.addChild (preset, 0, nullptr);
        }
        else
        {
            cp.setProperty (juce::Identifier ("path"), relativePresetPath, nullptr);
        }
    }

    /*
    Returns relative preset path
    */
    inline juce::String getPreset (const juce::ValueTree& state)
    {
        auto cp = state.getChildWithName (juce::Identifier ("CurrentPreset"));
        if (cp.isValid() && cp.hasProperty (juce::Identifier ("path")))
        {
            return cp.getPropertyAsValue (juce::Identifier ("path"), nullptr).toString();
        }
        else
        {
            return "";
        }
    }
} // namespace AudioProcessorStateUtil
} // namespace onsen
