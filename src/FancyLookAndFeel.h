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

    Typeface::Ptr getTypefaceForFont (const Font& f) override
    {
        return getCustomFont().getTypeface();
    }

    static const Font getCustomFont()
    {
        // Recursive-Regular.ttf
        static auto typeface = Typeface::createSystemTypefaceFor (BinaryData::RecursiveRegular_ttf, BinaryData::RecursiveRegular_ttfSize);
        return Font (typeface);
    }

private:
};
} // namespace onsen