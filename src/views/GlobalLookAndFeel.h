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
Dark
{
    0xff3b3b3b,
    0xff2e2e2e,
    0xff3b3b3b,
    0xff949494,
    0xffffffff,
    0xff42a2c8,
    0xffffffff,
    0xff181f22,
    0xffffffff
}

*/

#include <JuceHeader.h>
namespace onsen
{
//==============================================================================
class GlobalLookAndFeel : public juce::LookAndFeel_V4
{
    using Laf = juce::LookAndFeel_V4;

public:
    GlobalLookAndFeel() : colorScheme (
        { 0xff282423, 0xff282423, 0xff282423, 0xff949494, 0xffffffff, 0xff42a2c8, 0xffffffff, 0xff181f22, 0xffffffff })
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
