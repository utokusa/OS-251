/*
  ==============================================================================

   IntParamLookAndFeel

  ==============================================================================
*/

#pragma once

#include "../../DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class IntParamLookAndFeel : public LookAndFeel_V4
{
public:
    static constexpr int intParamWidth = 55;
    static constexpr int intParamHeight = 40;

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;
    Label* createSliderTextBox (Slider& slider) override;

private:
    juce::Image IntParamFilmStrip;
};
} // namespace onsen