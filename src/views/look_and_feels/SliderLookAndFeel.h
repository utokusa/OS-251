/*
  ==============================================================================

   SliderLookAndFeel

  ==============================================================================
*/

#pragma once

#include "../../DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class SliderLookAndFeel : public LookAndFeel_V4
{
public:
    static constexpr int sliderWidth = 60;
    static constexpr int sliderHeight = 220;

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&) override;
    Slider::SliderLayout getSliderLayout (Slider&) override;

private:
    static constexpr float thumbWidth = 42.0f;
    static constexpr float thumbHeight = 42.0f;

    const Rectangle<int> textBoxBounds = Rectangle<int> (30, 0, 70, 14);
    const Rectangle<int> sliderBounds = Rectangle<int> (11, 32, 40, 145);
};
} // namespace onsen