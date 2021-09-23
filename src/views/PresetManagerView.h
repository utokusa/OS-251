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
//==============================================================================
class PresetManagerView : public reactjuce::View, public juce::Button::Listener
{
public:
    PresetManagerView() : presetManager(), currentPresetMenuItemId (1)
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
        presetMenu.setTextWhenNothingSelected ("");
        loadPresetMenu();
        presetMenu.onChange = [this] {
            auto selectedItemId = presetMenu.getSelectedId();
            auto presetFiles = presetManager.getPresets();
            int presetFileIdx = presetArrayIdx (selectedItemId);
            if (presetFileIdx >= presetFiles.size())
            {
                // Selected item was an operation menu like "Save as".
                presetMenu.setSelectedId (currentPresetMenuItemId, juce::NotificationType::dontSendNotification);
                return;
            }

            if (presetManager.getCurrentPresetFile() == presetFiles[presetFileIdx])
            {
                // Avoid to undo changes on current preset
                return;
            }
            currentPresetMenuItemId = selectedItemId;
            presetManager.loadPreset (presetFiles[presetFileIdx]);
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
            auto presetFiles = presetManager.getPresets();
            presetManager.loadPreset (presetFiles[presetArrayIdx (selectedItemId)]);
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

    juce::ComboBox presetMenu;
    juce::PopupMenu factoryPresetMenu;
    juce::PopupMenu userPresetMenu;
    int currentPresetMenuItemId;
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
        // TODO: Need acync IO for scanning presets?
        presetManager.scanPresets();
        presetMenu.clear (juce::NotificationType::dontSendNotification);
        factoryPresetMenu.clear();
        userPresetMenu.clear();
        int id = 1; // Item ID should start from 1

        // Default preset
        currentPresetMenuItemId = 1; // default
        presetMenu.addItem ("Default", id++);

        // Factory presets
        auto factoryPresetFiles = presetManager.getFactoryPresets();
        {
            int i = 0;
            createPresetMenuTree (factoryPresetMenu, id, factoryPresetFiles, i, presetManager.getFactoryPresetDir());
        }
        presetMenu.getRootMenu()->addSubMenu ("Factory Presets", factoryPresetMenu);

        // User presets
        auto userPresetFiles = presetManager.getUserPresets();
        {
            int i = 0;
            createPresetMenuTree (userPresetMenu, id, userPresetFiles, i, presetManager.getUserPresetDir());
        }
        presetMenu.getRootMenu()->addSubMenu ("User Presets", userPresetMenu);

        // --- Operation menu ---
        // Save
        juce::PopupMenu::Item saveItem;
        saveItem.itemID = id++;
        saveItem.text = "Save";
        saveItem.action = [this]() {
            presetManager.savePreset (presetManager.getCurrentPresetFile());
        };
        presetMenu.getRootMenu()->addItem (saveItem);

        // Save as
        juce::PopupMenu::Item saveAsItem;
        saveAsItem.itemID = id++;
        saveAsItem.text = "Save as...";
        saveAsItem.action = [this]() {
            chooser = std::make_unique<juce::FileChooser> ("Save as...",
                                                           presetManager.getUserPresetDir(),
                                                           "*.oapreset");
            auto chooserFlag = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
            chooser->launchAsync (chooserFlag, [this] (const juce::FileChooser& chooser) {
                juce::File file (chooser.getResult());
                if (file.getFullPathName() == "")
                    return;
                presetManager.savePreset (file);
                loadPresetMenu();
                auto presetFiles = presetManager.getPresets();
                int idx = presetFiles.indexOf (file);
                presetMenu.setSelectedId (presetMenuItemId (idx));
            });
        };
        presetMenu.getRootMenu()->addItem (saveAsItem);

        // Go to preset folder
        juce::PopupMenu::Item goToPresetFolderItem;
        goToPresetFolderItem.itemID = id++;
        goToPresetFolderItem.text = "Go to Preset Folder...";
        goToPresetFolderItem.action = [this]() {
            juce::File dir = presetManager.getUserPresetDir();
            dir.revealToUser();
        };
        presetMenu.getRootMenu()->addItem (goToPresetFolderItem);

        // Rescan presets
        juce::PopupMenu::Item rescanPresetsItem;
        rescanPresetsItem.itemID = id++;
        rescanPresetsItem.text = "Rescan Presets";
        rescanPresetsItem.action = [this]() {
            auto currentFile = presetManager.getCurrentPresetFile();
            loadPresetMenu();
            if (currentFile.existsAsFile())
            {
                auto presetFiles = presetManager.getPresets();
                int idx = presetFiles.indexOf (currentFile);
                currentPresetMenuItemId = presetMenuItemId (idx);
                presetMenu.setSelectedId (currentPresetMenuItemId, juce::NotificationType::dontSendNotification);
            }
            else
            {
                presetMenu.setSelectedId (1, juce::NotificationType::dontSendNotification);
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
        auto presets = presetManager.getPresets();
        if (presetArrayIdx (selectedItemId + 1) >= presets.size())
            return;
        presetMenu.setSelectedId (selectedItemId + 1);
    }

    int presetArrayIdx (int presetMenuItemId)
    {
        // juce::Combobox's ID starts from 1
        return presetMenuItemId - 1;
    }

    int presetMenuItemId (int presetArrayIdx)
    {
        // juce::Combobox's ID starts from 1
        return presetArrayIdx + 1;
    }
};
} // namespace onsen
