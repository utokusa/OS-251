/*
  ==============================================================================

   KnobLookAndFeel

  ==============================================================================
*/

#pragma once

#include "../../DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class KnobLookAndFeel : public LookAndFeel_V4
{
public:
    static constexpr int knobWidth = 60;
    static constexpr int knobHeight = 60;

    KnobLookAndFeel()
    {
        knobFilmStrip = juce::ImageCache::getFromMemory (BinaryData::knob_png, BinaryData::knob_pngSize);
    }
    void drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider) override;

private:
    juce::Image knobFilmStrip;
};
} // namespace onsen