/*
  ==============================================================================

   Preset Manager View

  ==============================================================================
*/

#pragma once

#include "../services/PresetManager.h"
#include <JuceHeader.h>
#include <iostream>
#include <unordered_map>

namespace onsen
{
class PresetManagerView : public reactjuce::View, public juce::Button::Listener
{
public:
    PresetManagerView() : presetManager(), currentPresetMenuIdx (1)
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
                currentPresetMenuIdx = selectedItemId;
            }
            else if (presetFileIdx <= presetFiles.size() - 1)
            {
                presetManager.loadPreset (presetFiles[presetFileIdx]);
                currentPresetMenuIdx = selectedItemId;
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
    int currentPresetMenuIdx;
    std::unique_ptr<juce::FileChooser> chooser;
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

        juce::PopupMenu::Item saveItem;
        saveItem.itemID = id++;
        saveItem.text = "Save";
        saveItem.action = [this]() {
            presetMenu.setSelectedId (currentPresetMenuIdx);
            std::cout << "saving... " << presetMenu.getSelectedItemIndex() << std::endl;
            auto files = presetManager.getUserPresets();
            auto file = files[presetArrayIdx (currentPresetMenuIdx)];
            presetManager.savePreset (file);
        };
        presetMenu.getRootMenu()->addItem (saveItem);

        juce::PopupMenu::Item saveAsItem;
        saveAsItem.itemID = id++;
        saveAsItem.text = "Save as...";
        saveAsItem.action = [this]() {
            std::cout << "save as... " << presetMenu.getSelectedItemIndex() << std::endl;
            chooser = std::make_unique<juce::FileChooser> ("Save as...",
                                                           presetManager.getUserPresetDir(),
                                                           "*.oapreset");
            auto chooserFlag = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
            chooser->launchAsync (chooserFlag, [this] (const juce::FileChooser& chooser) {
                juce::File file (chooser.getResult());
                presetManager.savePreset (file);
                presetMenu.setSelectedId (currentPresetMenuIdx);
            });
        };
        presetMenu.getRootMenu()->addItem (saveAsItem);

        juce::PopupMenu::Item goToPresetFolderItem;
        goToPresetFolderItem.itemID = id++;
        goToPresetFolderItem.text = "Go to Preset Folder...";
        goToPresetFolderItem.action = [this]() {
            presetMenu.setSelectedId (currentPresetMenuIdx);
            std::cout << "Go to preset folder... " << presetMenu.getSelectedItemIndex() << std::endl;
            juce::File dir = presetManager.getUserPresetDir();
            dir.revealToUser();
        };
        presetMenu.getRootMenu()->addItem (goToPresetFolderItem);

        juce::PopupMenu::Item rescanPresetsItem;
        rescanPresetsItem.itemID = id++;
        rescanPresetsItem.text = "Rescan Presets";
        rescanPresetsItem.action = [this]() {
            presetMenu.setSelectedId (currentPresetMenuIdx);
            std::cout << "Rescan Presets... " << presetMenu.getSelectedItemIndex() << std::endl;
        };
        presetMenu.getRootMenu()->addItem (rescanPresetsItem);
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
