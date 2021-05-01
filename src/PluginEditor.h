/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#pragma once

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
    [[maybe_unused]] Os251AudioProcessor& audioProcessor;

    juce::AudioProcessorValueTreeState& parameters;
    reactjuce::ReactApplicationRoot appRoot;
    std::unique_ptr<reactjuce::AppHarness> harness;
    juce::String tmpUiBuldlePath;

    static constexpr int bodyWidth = 900;
    static constexpr int bodyHeight = 625;
    static constexpr int headerHeight = 25;
    static constexpr int appWidth = bodyWidth;
    static constexpr int appHeight = bodyHeight + headerHeight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};