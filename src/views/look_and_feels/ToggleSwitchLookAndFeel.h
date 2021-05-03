/*
  ==============================================================================

   ToggleSwitchLookAndFeel

  ==============================================================================
*/

#pragma once

#include "../../DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class ToggleSwitchLookAndFeel : public LookAndFeel_V4
{
public:
    static constexpr int toggleSwitchWidth = 48;
    static constexpr int toggleSwitchHeight = 27;

    ToggleSwitchLookAndFeel()
    {
        ToggleSwitchOffImage = juce::ImageCache::getFromMemory (BinaryData::toggle_switch_off_png, BinaryData::toggle_switch_off_pngSize);
        ToggleSwitchOnImage = juce::ImageCache::getFromMemory (BinaryData::toggle_switch_on_png, BinaryData::toggle_switch_on_pngSize);
    }
    void drawTickBox (Graphics& g, Component&, float x, float y, float width, float height, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

private:
    juce::Image ToggleSwitchOffImage;
    juce::Image ToggleSwitchOnImage;
};
} // namespace onsen