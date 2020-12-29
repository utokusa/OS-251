/*
  ==============================================================================

    ParamSlider.h
    Created: 21 Oct 2020 12:17:51pm
    Author:  Utokusa

  ==============================================================================
*/

#pragma once

#include <utility>
#include <JuceHeader.h>

class ParamSlider : public juce::Component
{
public:
    ParamSlider (juce::AudioProcessorValueTreeState& params, juce::String _paramId)
        : parameters (params), paramId (std::move (_paramId))
    {
        slider.setSliderStyle (juce::Slider::RotaryVerticalDrag);
        slider.setTextBoxIsEditable (false);
        label.setText (parameters.getParameter (paramId)->name, juce::dontSendNotification);
        addAndMakeVisible (slider);
        sliderAttachment = std::make_unique<SliderAttachment> (parameters, paramId, slider);
        addAndMakeVisible (label);
        setSize (150, 120);
    }
    ParamSlider() = delete;

    void paint (juce::Graphics& g) override {}

    void resized() override
    {
        // Lay out parameters
        constexpr int sliderW = 150;
        constexpr int sliderH = 100;
        constexpr int sliderTxtW = 80;
        constexpr int sliderTxtH = 20;
        // {label's Y} = {slider's Y} + {this value}
        constexpr int sliderLabelDistY = 58;
        constexpr int labelW = 100;
        constexpr int labelH = 30;

        slider.setBounds (0, 0, sliderW, sliderH);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
        label.setBounds (0, sliderLabelDistY, labelW, labelH);
    }

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::String paramId;
    juce::Slider slider;
    juce::Label label;
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> sliderAttachment;
};