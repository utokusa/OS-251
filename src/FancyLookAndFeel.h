/*
  ==============================================================================

   FancyLookAndFeel

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
namespace onsen
{
//==============================================================================
class FancyLookAndFeel : public juce::LookAndFeel_V4
{
public:
    FancyLookAndFeel()
    {
        setDefaultSansSerifTypeface (getCustomFont().getTypeface());
    }

    ~FancyLookAndFeel() {}

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
};
} // namespace onsen
