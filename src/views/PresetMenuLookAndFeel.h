/*
  ==============================================================================

   Preset Menu Look and Feel

  ==============================================================================
*/

/*
-------------------
-- Color Schemas --
-------------------
{
    windowBackground,
    widgetBackground,
    menuBackground,
    outline,
    defaultText,
    defaultFill,
    highlightedText,
    highlightedFill,
    menuText
}
*/

#pragma once

#include "colors.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class PresetMenuLookAndFeel : public juce::LookAndFeel_V4
{
    using Laf = juce::LookAndFeel_V4;

public:
    PresetMenuLookAndFeel() : colorScheme (
        { colors::backgroundColor,
          colors::backgroundColorDark,
          colors::darkGray2,
          colors::textColorDark,
          colors::textColor,
          colors::backgroundColorDark,
          colors::textColor,
          colors::almostBlack,
          colors::textColor })
    {
        setColourScheme (colorScheme);
    }

    ~PresetMenuLookAndFeel() {}

    void drawComboBox (juce::Graphics& g, int width, int height, bool isButtonDown, int buttonX, int buttonY, int buttonW, int buttonH, juce::ComboBox& box) override
    {
        auto cornerSize = 4.0f;
        juce::Rectangle<int> boxBounds (0, 0, width, height);

        g.setColour (box.findColour (juce::ComboBox::backgroundColourId));
        g.fillRoundedRectangle (boxBounds.toFloat(), cornerSize);

        g.setColour (box.findColour (juce::ComboBox::outlineColourId));
        g.drawRoundedRectangle (boxBounds.toFloat().reduced (0.5f, 0.5f), cornerSize, 2.0f);
    }

    void positionComboBoxText (juce::ComboBox& box, juce::Label& label) override
    {
        label.setBounds (1, 1, box.getWidth() - 4, box.getHeight() - 2);

        label.setFont (getComboBoxFont (box));
    }

    juce::Font getComboBoxFont (juce::ComboBox& box) override
    {
        return { std::min (13.0f, static_cast<float> (box.getHeight()) * 0.85f) };
    }

    juce::Path getTickShape (float height) override
    {
        juce::Path path;
        path.addEllipse (0, 0, height, height);

        return path;
    }

private:
    Laf::ColourScheme colorScheme;
};
} // namespace onsen
