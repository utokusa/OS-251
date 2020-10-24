/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "ParamSlider.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <array>

//==============================================================================
/**
*/
class Os251AudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    Os251AudioProcessorEditor (Os251AudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~Os251AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    __unused Os251AudioProcessor& audioProcessor;

    juce::AudioProcessorValueTreeState& parameters;

    static constexpr int numRows = 3;
    static constexpr int numCols = 4;
    template <class T>
    using SliderMatrix = std::array<std::array<T, numCols>, numRows>;
    SliderMatrix<juce::String> paramIdList { { { "sinGain", "squareGain", "sawGain", "" },
                                               { "attack", "decay", "sustain", "release" },
                                               { "frequency", "resonance", "filterEnv", "" } } };
    using ParamSliderPtr = std::unique_ptr<ParamSlider>;
    SliderMatrix<ParamSliderPtr> sliderList;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};
