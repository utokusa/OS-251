/*
  ==============================================================================

   GlobalLookAndFeel

  ==============================================================================
*/

#pragma once

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

#include "colors.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class GlobalLookAndFeel : public juce::LookAndFeel_V4
{
    using Laf = juce::LookAndFeel_V4;

public:
    GlobalLookAndFeel() : colorScheme (
        {
            colors::darkGray,
            colors::darkGray,
            colors::darkGray2,
            colors::grey,
            colors::textColor,
            colors::blue,
            colors::textColor,
            colors::almostBlack,
            colors::textColor,
        }),
#if JUCE_WINDOWS
                          // For Windows we use edited version of Recursive-Regular.ttf
                          typeface (
                              juce::Typeface::createSystemTypefaceFor (
                                  BinaryData::Os251Font125Percent_ttf,
                                  BinaryData::Os251Font125Percent_ttfSize))
#else
                          // Recursive-Regular.ttf
                          typeface (
                              juce::Typeface::createSystemTypefaceFor (
                                  BinaryData::RecursiveRegular_ttf,
                                  BinaryData::RecursiveRegular_ttfSize))
#endif
    {
        setDefaultSansSerifTypeface (typeface);
        setColourScheme (colorScheme);
    }

    ~GlobalLookAndFeel() {}

    juce::Typeface::Ptr getTypefaceForFont (const juce::Font& f) override
    {
        return typeface;
    }

    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override
    {
        const auto fill = juce::Colour (colors::primaryColor);
        const auto outline = juce::Colour (colors::primaryColorDark);

        constexpr auto padding = 5.0;
        const auto bounds = juce::Rectangle<int> (x, y, width, height).toFloat().reduced (padding);
        const auto radius = fmin (bounds.getWidth(), bounds.getHeight()) / 2.0;
        const auto rotaryCurrentAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        const auto lineWidth = 3.0;

        if (slider.isEnabled())
        {
            juce::Path fullArc;
            fullArc.addCentredArc (bounds.getCentreX(), bounds.getCentreY(), radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);

            g.setColour (outline);
            g.strokePath (fullArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square));

            juce::Path valueArc;
            valueArc.addCentredArc (bounds.getCentreX(), bounds.getCentreY(), radius, radius, 0.0f, rotaryStartAngle, rotaryCurrentAngle, true);

            g.setColour (fill);
            g.strokePath (valueArc, juce::PathStrokeType (lineWidth, juce::PathStrokeType::curved, juce::PathStrokeType::square));
        }
    }

    juce::Slider::SliderLayout getSliderLayout (juce::Slider& slider) override
    {
        auto const localBounds = slider.getLocalBounds();
        juce::Slider::SliderLayout layout;
        layout.textBoxBounds = localBounds.withHeight (14).withCentre (localBounds.getCentre());
        layout.sliderBounds = localBounds;
        return layout;
    }

    juce::Label* createSliderTextBox (juce::Slider& slider) override
    {
        auto l = juce::LookAndFeel_V4::createSliderTextBox (slider);
        l->setInterceptsMouseClicks (false, false);
        l->setColour (juce::Label::outlineColourId, juce::Colours::transparentWhite);
        return l;
    }

private:
    Laf::ColourScheme colorScheme;
    juce::Typeface::Ptr typeface;
};
} // namespace onsen
