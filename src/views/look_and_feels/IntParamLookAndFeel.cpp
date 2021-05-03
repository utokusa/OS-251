/*
  ==============================================================================

   IntParamLookAndFeel

  ==============================================================================
*/

#include "IntParamLookAndFeel.h"

namespace onsen
{
//==============================================================================
void IntParamLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    // Do Nothing here
}

Label* IntParamLookAndFeel::createSliderTextBox (Slider& slider)
{
    auto* label = juce::LookAndFeel_V3::createSliderTextBox (slider);
    label->setColour (juce::Label::textColourId, juce::Colour (0xFFB7BBAD));
    label->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite);
    label->setColour (juce::Label::outlineWhenEditingColourId, juce::Colours::transparentWhite);
    label->setColour (juce::TextEditor::backgroundColourId, juce::Colours::transparentWhite);

    auto font = Font (8.0f);
    label->setFont (font);

    return label;
}

} // namespace onsen