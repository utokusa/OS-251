#include "SliderLookAndFeel.h"

//==============================================================================
void SliderLookAndFeel::drawLinearSlider (Graphics& g, int x, int y, int width, int height, float sliderPos, float minSliderPos, float maxSliderPos, const Slider::SliderStyle, Slider&)
{
    Image thumb = ImageCache::getFromMemory (BinaryData::slider_thumb_png, BinaryData::slider_thumb_pngSize);
    Rectangle<float> knobArea (x - thumbWidth / 2, sliderPos - thumbWidth / 2, thumbWidth, thumbHeight);
    g.drawImage (thumb, knobArea, RectanglePlacement::Flags::xLeft);
}

Slider::SliderLayout SliderLookAndFeel::getSliderLayout (Slider&)
{
    Slider::SliderLayout layout;
    layout.sliderBounds = sliderBounds;

    return layout;
}
