#include "../DspCommon.h"
#include <JuceHeader.h>

class BodyView : public reactjuce::View
{
    void paint (juce::Graphics& g) override
    {
        // Do your own paint routine like usual.
        // You can also treat this whole class instance like your normal juce::Components. Add children, `addAndMakeVisible`,
        // `resized` and everything!
        Image backgroundImage = ImageCache::getFromMemory (BinaryData::background_png, BinaryData::background_pngSize);
        g.drawImageWithin (backgroundImage, 0, 0, 900, 625, RectanglePlacement::Flags::xLeft, false);
    }
};