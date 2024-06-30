/*
  ==============================================================================

   JUCE Plugin Editor for reace-juce

  ==============================================================================
*/

#pragma once

#include "JuceAudioProcessorState.h"
#include "PluginProcessor.h"
#include "views/ClippingIndicatorView.h"
#include "views/PresetManagerView.h"
#include <JuceHeader.h>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

//==============================================================================
/**
*/
class Os251AudioProcessorEditor : public juce::AudioProcessorEditor,
                                  public juce::AudioProcessorParameter::Listener,
                                  public juce::Timer
{
public:
    Os251AudioProcessorEditor (Os251AudioProcessor&, juce::AudioProcessorValueTreeState&, onsen::PresetManager&, onsen::ISynthUi* synthUi);
    ~Os251AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;
    void timerCallback() override;

private:
    //==============================================================================

    Os251AudioProcessor& audioProcessor;
    onsen::PresetManager& presetManager;
    onsen::ISynthUi* const synthUi;
    juce::GenericAudioProcessorEditor genericEditor;
    onsen::PresetManagerView presetManagerView;

    std::unordered_map<juce::String, juce::AudioProcessorParameter*> parameterById;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};
