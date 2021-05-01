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

    sliderSin.setBounds (75, 80, SliderLookAndFeel::sliderWidth, SliderLookAndFeel::sliderHeight);
}
} // namespace onsen
