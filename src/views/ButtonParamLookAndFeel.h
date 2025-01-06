/*
  ==============================================================================

   ButtonParamLookAndFeel

  ==============================================================================
*/

#pragma once

#include "colors.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class ButtonParamLookAndFeel : public juce::LookAndFeel_V4
{
    using Laf = juce::LookAndFeel_V4;

public:
    ButtonParamLookAndFeel()
    {
    }

    ~ButtonParamLookAndFeel() {}

    void drawButtonBackground (juce::Graphics& g, juce::Button& button, const juce::Colour& backgroundColour, bool isMouseOverButton, bool isButtonDown) override
    {
        auto bounds = button.getLocalBounds().toFloat();
        auto buttonBounds = bounds.withHeight (36).withCentre (bounds.getCentre());
        auto baseColour = backgroundColour;

        if (isMouseOverButton)
            baseColour = baseColour.brighter (0.1f);

        if (isButtonDown)
            baseColour = baseColour.darker (0.2f);

        g.setColour (baseColour);
        g.fillRoundedRectangle (buttonBounds, 6.0f);

        if (button.getToggleStateValue() == true)
            g.setColour (button.findColour (juce::TextButton::textColourOnId));
        else
            g.setColour (button.findColour (juce::TextButton::textColourOffId));

        g.drawRoundedRectangle (buttonBounds.reduced (1.0f), 6.0f, 2.0f);
    }
};
} // namespace onsen
