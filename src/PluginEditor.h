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
    enum class ParamType
    {
        SLIDER,
        BUTTON,
    };

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    Os251AudioProcessor& audioProcessor;
    onsen::PresetManager& presetManager;
    onsen::ISynthUi* const synthUi;
    juce::AudioProcessorValueTreeState& valueTreeState;

    onsen::PresetManagerView presetManagerView;
    onsen::ClippingIndicatorView clippingIndicatorView;

    std::unordered_map<juce::String, juce::AudioProcessorParameter*> parameterById;

    //==============================================================================

    juce::Slider s_freq;
    juce::Label s_freqLabel;
    std::unique_ptr<SliderAttachment> s_freqAttachment;

    juce::TextButton b_chorusOn;
    juce::Label b_chorusOnLabel;
    std::unique_ptr<ButtonAttachment> b_chorusOnAttachment;

    std::vector<std::pair<std::string, ParamType>> paramLayout {
        { "sinGain", ParamType::SLIDER },
        { "squareGain", ParamType::SLIDER },
        { "sawGain", ParamType::SLIDER },
        { "subSquareGain", ParamType::SLIDER },

        { "noiseGain", ParamType::SLIDER },
        { "shape", ParamType::SLIDER },
        { "lfoShape", ParamType::SLIDER },
        { "numVoices", ParamType::SLIDER },

        { "attack", ParamType::SLIDER },
        { "decay", ParamType::SLIDER },
        { "sustain", ParamType::SLIDER },
        { "release", ParamType::SLIDER },

        { "frequency", ParamType::SLIDER },
        { "resonance", ParamType::SLIDER },
        { "filterEnv", ParamType::SLIDER },
        { "lfoFilterFreq", ParamType::SLIDER },

        { "syncOn", ParamType::BUTTON },
        { "rate", ParamType::SLIDER },
        { "rateSync", ParamType::SLIDER },
        { "lfoPhase", ParamType::SLIDER },

        { "lfoDelay", ParamType::SLIDER },
        { "unisonOn", ParamType::BUTTON },
        { "hpfFreq", ParamType::SLIDER },
        { "chorusOn", ParamType::BUTTON },

        { "lfoPitch", ParamType::SLIDER },
        { "pitchBendWidth", ParamType::SLIDER },
        { "portamento", ParamType::SLIDER },
        { "masterOctaveTune", ParamType::SLIDER },

        { "masterSemitoneTune", ParamType::SLIDER },
        { "masterFineTune", ParamType::SLIDER },
        { "envForAmpOn", ParamType::BUTTON },
        { "masterVolume", ParamType::SLIDER },
    };

    std::unordered_map<std::string, std::unique_ptr<juce::Slider>> sliders;
    std::unordered_map<std::string, std::unique_ptr<juce::Label>> sliderLabels;
    std::unordered_map<std::string, std::unique_ptr<SliderAttachment>> sliderAttachment;
    std::unordered_map<std::string, std::unique_ptr<juce::TextButton>> buttons;
    std::unordered_map<std::string, std::unique_ptr<juce::Label>> buttonLabels;
    std::unordered_map<std::string, std::unique_ptr<ButtonAttachment>> buttonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};
