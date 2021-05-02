/*
  ==============================================================================

   KnobLookAndFeel

  ==============================================================================
*/

#include "KnobLookAndFeel.h"

namespace onsen
{
//==============================================================================
void KnobLookAndFeel::drawRotarySlider (juce::Graphics& g, int x, int y, int width, int height, float sliderPos, const float rotaryStartAngle, const float rotaryEndAngle, Slider& slider)
{
    const float rotation = (slider.getValue() - slider.getMinimum())
                           / (slider.getMaximum() - slider.getMinimum());
    const int frames = 256;
    const int frameId = (int) ceil (rotation * ((double) frames - 1.0));

    int imgWidth = knobFilmStrip.getWidth();
    int imgHeight = knobFilmStrip.getHeight() / frames;
    g.drawImage (knobFilmStrip, x, y, width, height, 0, frameId * imgHeight, imgWidth, imgHeight);

}
} // namespace onsen