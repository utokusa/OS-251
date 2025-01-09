
/*
  ==============================================================================

   Clipping Indicator View

  ==============================================================================
*/

#pragma once

#include "../synth/ISynthUi.h"
#include "colors.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
// When audio is clipped, the indicator color becomes red
class ClippingIndicatorView : public juce::Component, juce::Timer
{
public:
    ClippingIndicatorView (ISynthUi* synthUi);
    void paint (juce::Graphics& g) override; // Lights the LED if the clipper is working.
    void resized() override {}
    void timerCallback() override { repaint(); }

private:
    //=============================================================================
    ISynthUi* const synthUi;
    static constexpr float largerCircleDiameter = 14.0f;
    static constexpr float smallerCircleDiameter = 8.0f;
    static constexpr float diffRadius = (largerCircleDiameter - smallerCircleDiameter) / 2.0f;
    static constexpr float circleThickness = 2.0f;
    static constexpr float margin = 5.0f;
    static constexpr float textWidth = 50.0f;
    static constexpr float textHeight = 20.0f;
    static constexpr unsigned int clippingColorId = colors::red; // LED is lit.
    static constexpr unsigned int nonClippingColorId = colors::primaryColorDark; // LED is not lit.
};
} // namespace onsen
