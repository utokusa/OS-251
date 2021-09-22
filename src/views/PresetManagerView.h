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
        revertButton.removeListener (this);
        prevButton.removeListener (this);
        nextButton.removeListener (this);
    }

    void paint (juce::Graphics& g) override
    {
    }

    void resized() override
    {
        revertButton.setBounds (getWidth() * 2 / 8, 0, getWidth() / 8, 15);
        prevButton.setBounds (getWidth() * 3 / 8, 0, getWidth() / 8, 15);
        nextButton.setBounds (getWidth() * 4 / 8, 0, getWidth() / 8, 15);
        presetMenu.setBounds (getWidth() * 5 / 8, 0, getWidth() * 3 / 8, 15);
    }

    void buttonClicked (juce::Button* button) override
    {
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
    juce::TextButton revertButton;
    juce::TextButton prevButton;
    juce::TextButton nextButton;

    juce::ComboBox presetMenu; // TODO: need to do something like presetMenu.removeListener()?
    juce::PopupMenu userPresetMenu;
    int currentPresetMenuIdx;
    std::unique_ptr<juce::FileChooser> chooser;
    //==============================================================================
    void createPresetMenuTree (juce::PopupMenu& menu,
                               int& itemId,
                               juce::Array<juce::File>& presets,
                               int& presetIdx,
                               juce::File currentDir)
    {
        while (presetIdx < presets.size())
        {
            // TODO: Confirm that presets[i] is a file, not a dir.
            if (currentDir == presets[presetIdx].getParentDirectory())
            {
                juce::PopupMenu::Item presetItem;
                presetItem.itemID = itemId;
                presetItem.text = PresetManager::getPresetName (presets[presetIdx]);
                menu.addItem (presetItem);
                itemId++;
                presetIdx++;
            }
            else if (presets[presetIdx].isAChildOf (currentDir))
            {
                juce::PopupMenu childMenu;
                auto relativePathToPreset = presets[presetIdx].getRelativePathFrom (currentDir);
                auto separator = juce::File::getSeparatorString();
                auto tokens = juce::StringArray::fromTokens (relativePathToPreset, separator, juce::StringRef());
                auto childMenuName = tokens[0];
                createPresetMenuTree (childMenu, itemId, presets, presetIdx, currentDir.getChildFile (childMenuName));
                menu.addSubMenu (childMenuName, childMenu);
            }
            else
            {
                break;
            }
        }
    }

    void loadPresetMenu()
    {
        presetMenu.clear (juce::NotificationType::dontSendNotification);
        userPresetMenu.clear();
        int id = 1; // Item ID should start from 1
        currentPresetMenuIdx = 1; // default
        presetMenu.addItem ("default", id++);
        auto presetFiles = presetManager.scanUserPresets();
        {
            int i = 0;
            createPresetMenuTree (userPresetMenu, id, presetFiles, i, presetManager.getUserPresetDir());
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
            // TODO: Remove hard-coded "*.oapreset"
            chooser = std::make_unique<juce::FileChooser> ("Save as...",
                                                           presetManager.getUserPresetDir(),
                                                           "*.oapreset");
            auto chooserFlag = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
            chooser->launchAsync (chooserFlag, [this] (const juce::FileChooser& chooser) {
                juce::File file (chooser.getResult());
                presetManager.savePreset (file);
                loadPresetMenu();
                auto presetFiles = presetManager.getUserPresets();
                int idx = presetFiles.indexOf (file);
                // TODO: handle the case when currentFile.existsAsFile() == false here.
                presetMenu.setSelectedId (presetMenuId (idx));
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
            auto presetFiles = presetManager.getUserPresets();
            if (currentPresetMenuIdx == 1 /*default*/)
            {
                loadPresetMenu();
                presetMenu.setSelectedId (1);
            }
            else
            {
                auto currentFile = presetFiles[presetArrayIdx (currentPresetMenuIdx)];
                loadPresetMenu();
                if (currentFile.existsAsFile())
                {
                    auto presetFiles = presetManager.getUserPresets();
                    int idx = presetFiles.indexOf (currentFile);
                    // TODO: handle the case when currentFile.existsAsFile() == false here.
                    presetMenu.setSelectedId (presetMenuId (idx));
                }
                else
                {
                    presetMenu.setSelectedId (1);
                }
            }
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

    int presetMenuId (int presetArrayIdx)
    {
        return presetArrayIdx + 2;
    }
};
} // namespace onsen
