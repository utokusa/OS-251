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

#include <JuceHeader.h>
namespace onsen
{
//==============================================================================
class PresetMenuLookAndFeel : public juce::LookAndFeel_V4
{
    using Laf = juce::LookAndFeel_V4;

public:
    PresetMenuLookAndFeel() : colorScheme (
        { 0xff282423,
          0xff262123,
          0xff2e2e2e,
          0xffA99988,
          0xffE4D3B0,
          0xff42a2c8,
          0xffE4D3B0,
          0xff181f22,
          0xffE4D3B0 })
    {
        setColourScheme (colorScheme);
    }

    ~PresetMenuLookAndFeel() {}

private:
    Laf::ColourScheme colorScheme;
};
} // namespace onsen
