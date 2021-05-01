/*
  ==============================================================================

   Body

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"
#include "look_and_feels/SliderLookAndFeel.h"
#include <JuceHeader.h>

class BodyView : public reactjuce::View
{
public:
    BodyView (juce::AudioProcessorValueTreeState& params)
    {
        sliderSin.setLookAndFeel (&sliderLookAndFeel);
        sliderSin.setSliderStyle (Slider::LinearVertical);
        sliderSin.setMouseDragSensitivity (700);
        sliderSin.hideTextBox (true);
        addAndMakeVisible (sliderSin);
        s_freqAttachment.reset (new SliderAttachment (params, "sinGain", sliderSin));
    }

    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    SliderLookAndFeel sliderLookAndFeel;

    juce::Slider sliderSin;
    std::unique_ptr<SliderAttachment> s_freqAttachment;
};