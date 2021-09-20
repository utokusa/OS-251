/*
  ==============================================================================

   Preset Manager View

  ==============================================================================
*/

#include "../services/PresetManager.h"
#include <JuceHeader.h>

namespace onsen
{
class PresetManagerView : public reactjuce::View, public juce::Button::Listener
{
public:
    PresetManagerView() : cnt (0),
                          presetManager()
    {
        addAndMakeVisible (saveButton);
        saveButton.setButtonText ("Save");
        saveButton.addListener (this);

        addAndMakeVisible (loadButton);
        loadButton.setButtonText ("Load");
        loadButton.addListener (this);

        addAndMakeVisible (cntLabel);
        cntLabel.setColour (juce::Label::backgroundColourId, juce::Colours::black);
        cntLabel.setColour (juce::Label::textColourId, juce::Colours::white);
        cntLabel.setJustificationType (juce::Justification::centred);
    }

    ~PresetManagerView()
    {
        saveButton.removeListener (this);
        loadButton.removeListener (this);
    }

    void paint (juce::Graphics& g) override
    {
        g.fillAll (juce::Colour (0xA9, 0x99, 0x88));
    }

    void resized() override
    {
        saveButton.setBounds (0, 0, getWidth() / 3, 15);
        loadButton.setBounds (getWidth() / 3, 0, getWidth() / 3, 15);
        cntLabel.setBounds (getWidth() * 2 / 3, 0, getWidth() / 3, 15);
    }

    void buttonClicked (juce::Button* button) override
    {
        if (button == &saveButton)
        {
            cnt++;
            cntLabel.setText (juce::String (cnt), juce::dontSendNotification);

            presetManager.save();
        }

        if (button == &loadButton)
        {
            presetManager.load();
        }
    }

private:
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::Label cntLabel;
    int cnt;
    PresetManager presetManager;
};
} // namespace onsen
