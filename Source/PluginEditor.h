/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class Os251AudioProcessorEditor  : public juce::AudioProcessorEditor
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
    Os251AudioProcessor& audioProcessor;
    
    juce::AudioProcessorValueTreeState& parameters;
    
    juce::Slider sAttack;
    juce::Slider sDecay;
    juce::Slider sSustain;
    juce::Slider sRelease;
    
    juce::Label sAttackLabel;
    juce::Label sDecayLabel;
    juce::Label sSustainLabel;
    juce::Label sReleaseLabel;
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> sAttackAttachment;
    std::unique_ptr<SliderAttachment> sDecayAttachment;
    std::unique_ptr<SliderAttachment> sSustainAttachment;
    std::unique_ptr<SliderAttachment> sReleaseAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};
