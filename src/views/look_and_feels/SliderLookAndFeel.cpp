/*
  ==============================================================================

   SliderLookAndFeel

  ==============================================================================
*/

#include "SliderLookAndFeel.h"

namespace onsen
{
//==============================================================================
void SliderLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&)
{
    Image thumb = ImageCache::getFromMemory (BinaryData::slider_thumb_png, BinaryData::slider_thumb_pngSize);
    Rectangle<float> knobArea (x, sliderPos, thumbWidth, thumbHeight);
    g.drawImage (thumb, knobArea, RectanglePlacement::Flags::xLeft);
}

Slider::SliderLayout SliderLookAndFeel::getSliderLayout (Slider&)
{
    Slider::SliderLayout layout;
    layout.sliderBounds = sliderBounds;

    return layout;
}
} // namespace onsen