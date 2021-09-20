/*
  ==============================================================================

   Preset Manager View

  ==============================================================================
*/

#pragma once

#include "../services/PresetManager.h"
#include <JuceHeader.h>
#include <unordered_map>

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

        addAndMakeVisible (revertButton);
        revertButton.setButtonText ("Revert");
        revertButton.addListener (this);

        addAndMakeVisible (prevButton);
        prevButton.setButtonText ("Prev");
        prevButton.addListener (this);

        addAndMakeVisible (nextButton);
        nextButton.setButtonText ("Next");
        nextButton.addListener (this);

        addAndMakeVisible (presetMenu);
        presetMenu.setJustificationType (juce::Justification::centred);
        presetMenu.setTextWhenNothingSelected ("Default");
        loadPresetMenu();
        presetMenu.onChange = [this] {
            auto selectedItemId = presetMenu.getSelectedId();
            auto presetFiles = presetManager.getUserPresets();
            int presetFileIdx = presetArrayIdx (selectedItemId);
            if (selectedItemId == 1)
            {
                presetManager.loadDefaultPreset();
            }
            else if (presetFileIdx <= presetFiles.size() - 1)
            {
                presetManager.loadPreset (presetFiles[presetFileIdx]);
            }
        };
        presetMenu.setSelectedId (1);

        auto presetName = presetManager.loadDefaultPreset();
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
        saveButton.setBounds (0, 0, getWidth() / 8, 15);
        loadButton.setBounds (getWidth() / 8, 0, getWidth() / 8, 15);
        revertButton.setBounds (getWidth() * 2 / 8, 0, getWidth() / 8, 15);
        prevButton.setBounds (getWidth() * 3 / 8, 0, getWidth() / 8, 15);
        nextButton.setBounds (getWidth() * 4 / 8, 0, getWidth() / 8, 15);
        presetMenu.setBounds (getWidth() * 5 / 8, 0, getWidth() * 3 / 8, 15);
    }

    void buttonClicked (juce::Button* button) override
    {
        if (button == &saveButton)
        {
            presetManager.save();
        }

        if (button == &loadButton)
        {
            presetManager.load();
        }

        if (button == &revertButton)
        {
            int selectedItemId = presetMenu.getSelectedId();
            if (selectedItemId == 1)
            {
                presetManager.loadDefaultPreset();
            }
            else
            {
                auto presetFiles = presetManager.getUserPresets();
                presetManager.loadPreset (presetFiles[presetArrayIdx (selectedItemId)]);
            }
        }

        if (button == &prevButton)
        {
            prevClicked();
        }

        if (button == &nextButton)
        {
            nextClicked();
        }
    }

private:
    PresetManager presetManager;
    juce::TextButton saveButton;
    juce::TextButton loadButton;
    juce::TextButton revertButton;
    juce::TextButton prevButton;
    juce::TextButton nextButton;

    juce::ComboBox presetMenu;
    juce::PopupMenu userPresetMenu;
    //==============================================================================
    void loadPresetMenu()
    {
        int id = 1;
        presetMenu.addItem ("default", id++);
        auto presetFiles = presetManager.getUserPresets();
        for (auto presetFile : presetFiles)
        {
            juce::PopupMenu::Item presetItem;
            presetItem.itemID = id;
            presetItem.text = PresetManager::getPresetName (presetFile);
            userPresetMenu.addItem (presetItem);
            id++;
        }

        presetMenu.getRootMenu()->addSubMenu ("User Presets", userPresetMenu);
        presetMenu.addItem ("Save", id++);
        presetMenu.addItem ("Save as", id++);
    }

    void prevClicked()
    {
        int selectedItemId = presetMenu.getSelectedId();
        if (selectedItemId == 1)
            return;
        presetMenu.setSelectedId (selectedItemId - 1);
    }

    void nextClicked()
    {
        int selectedItemId = presetMenu.getSelectedId();
        auto presets = presetManager.getUserPresets();
        if (presetArrayIdx (selectedItemId + 1) >= presets.size())
            return;
        presetMenu.setSelectedId (selectedItemId + 1);
    }

    int presetArrayIdx (int itemId)
    {
        return itemId - 2;
    }
};
} // namespace onsen
