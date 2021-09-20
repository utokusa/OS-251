/*
  ==============================================================================

   Preset Manager View

  ==============================================================================
*/

#pragma once

#include "../services/PresetManager.h"
#include <JuceHeader.h>

namespace onsen
{
class PresetManagerView : public reactjuce::View, public juce::Button::Listener
{
public:
    PresetManagerView() : presetManager()
    {
        addAndMakeVisible (saveButton);
        saveButton.setButtonText ("Save");
        saveButton.addListener (this);

        addAndMakeVisible (loadButton);
        loadButton.setButtonText ("Load");
        loadButton.addListener (this);

        addAndMakeVisible (prevButton);
        prevButton.setButtonText ("Prev");
        prevButton.addListener (this);

        addAndMakeVisible (nextButton);
        nextButton.setButtonText ("Next");
        nextButton.addListener (this);

        addAndMakeVisible (presetNameLabel);
        presetNameLabel.setColour (juce::Label::backgroundColourId, juce::Colours::black);
        presetNameLabel.setColour (juce::Label::textColourId, juce::Colours::white);
        presetNameLabel.setJustificationType (juce::Justification::centred);

        auto presetName = presetManager.loadDefaultPreset();
        presetNameLabel.setText (presetName, juce::dontSendNotification);
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
        saveButton.setBounds (0, 0, getWidth() / 5, 15);
        loadButton.setBounds (getWidth() / 5, 0, getWidth() / 5, 15);
        presetNameLabel.setBounds (getWidth() * 2 / 5, 0, getWidth() / 5, 15);
        prevButton.setBounds (getWidth() * 3 / 5, 0, getWidth() / 5, 15);
        nextButton.setBounds (getWidth() * 4 / 5, 0, getWidth() / 5, 15);
    }

    void buttonClicked (juce::Button* button) override
    {
        if (button == &saveButton)
        {
            presetManager.save();
        }

        if (button == &loadButton)
        {
            auto presetName = presetManager.load();
            presetNameLabel.setText (presetName, juce::dontSendNotification);
        }

        if (button == &prevButton)
        {
            auto presetName = presetManager.loadPrev();
            presetNameLabel.setText (presetName, juce::dontSendNotification);
        }

        if (button == &nextButton)
        {
            auto presetName = presetManager.loadNext();
            presetNameLabel.setText (presetName, juce::dontSendNotification);
        }
    }

private:
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton prevButton;
    juce::TextButton nextButton;
    juce::Label presetNameLabel;
    PresetManager presetManager;
};
} // namespace onsen
