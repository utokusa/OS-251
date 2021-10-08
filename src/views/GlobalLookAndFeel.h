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

private:
    Laf::ColourScheme colorScheme;
    juce::Typeface::Ptr typeface;
};
} // namespace onsen
