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
    static constexpr int sliderWidth = 42;
    static constexpr int sliderHeight = 200;
    static constexpr float thumbWidth = 42.0f;
    static constexpr float thumbHeight = 42.0f;

    void drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&) override;
    Slider::SliderLayout getSliderLayout (Slider&) override;

private:
    const Rectangle<int> sliderBounds = Rectangle<int> (0, 0, sliderWidth, 148);
};
} // namespace onsen