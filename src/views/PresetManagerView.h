#include <JuceHeader.h>

namespace onsen
{
class PresetManagerView : public reactjuce::View
{
public:
    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour (0xA9, 0x99, 0x88));
    }
};
} // namespace onsen
