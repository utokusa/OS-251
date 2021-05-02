/*
  ==============================================================================

   BodyView

  ==============================================================================
*/

#include "BodyView.h"

namespace onsen
{
//==============================================================================
void BodyView::paint (juce::Graphics& g)
{
    // Do your own paint routine like usual.
    // You can also treat this whole class instance like your normal juce::Components. Add children, `addAndMakeVisible`,
    // `resized` and everything!
    juce::Image backgroundImage = juce::ImageCache::getFromMemory (BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageWithin (backgroundImage, 0, 0, 900, 625, juce::RectanglePlacement::Flags::xLeft, false);
}

void BodyView::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    for (int i = 0; i < numSlider; i++)
    {
        sliderArray[i].setBounds (
            sliderPositionArray[i].x - sliderXPositionAdjustment,
            sliderPositionArray[i].y + sliderYPositionAdjustment,
            SliderLookAndFeel::sliderWidth,
            SliderLookAndFeel::sliderHeight);
    }

    for (int i = 0; i < numKnob; i++)
    {
        knobArray[i].setBounds (
            knobPositionArray[i].x,
            knobPositionArray[i].y,
            KnobLookAndFeel::knobWidth,
            KnobLookAndFeel::knobHeight);
    }
}
} // namespace onsen
