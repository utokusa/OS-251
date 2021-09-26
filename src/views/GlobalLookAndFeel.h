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
            colors::white,
            colors::blue,
            colors::white,
            colors::almostBlack,
            colors::white,
        })
    {
        setDefaultSansSerifTypeface (getCustomFont().getTypeface());
        setColourScheme (colorScheme);
    }

    ~GlobalLookAndFeel() {}

    juce::Typeface::Ptr getTypefaceForFont (const juce::Font& f) override
    {
        return getCustomFont().getTypeface();
    }

    static const juce::Font getCustomFont()
    {
        // Recursive-Regular.ttf
        static auto typeface = juce::Typeface::createSystemTypefaceFor (BinaryData::RecursiveRegular_ttf, BinaryData::RecursiveRegular_ttfSize);
        return juce::Font (typeface);
    }

private:
    Laf::ColourScheme colorScheme;
};
} // namespace onsen
