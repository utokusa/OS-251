/*
  ==============================================================================

   Preset Manager View

  ==============================================================================
*/

/*
Our preset menu looks like...
============================
Default                      <--- ItemID=1
Factory Presets
User Presets
---------------------------- <--- Operation items are placed below here
Save
Save as...
Go to Preset Folder...
Rescan Presets
============================

Prev, Next, Revert are normal buttons.
*/

#pragma once

#include "../services/PresetManager.h"
#include <JuceHeader.h>
#include <functional>
#include <string>
#include <unordered_map>

namespace onsen
{
//==============================================================================
class PresetManagerView : public reactjuce::View, public juce::Button::Listener
{
    struct JuceFileHash
    {
        size_t operator() (const juce::File a) const
        {
            return std::hash<std::string>() (a.getFullPathName().toStdString());
        }
    };
    //==============================================================================
public:
    PresetManagerView() : presetManager(),
                          itemIdByPreset(),
                          presetByItemId(),
                          saveItem(),
                          saveAsItem(),
                          goToPresetFolderItem(),
                          rescanPresetsItem(),
                          doNothingOnPresetMenuChangeCallback (false)
    {
        addAndMakeVisible (prevButton);
        prevButton.setButtonText ("Prev");
        prevButton.addListener (this);

        addAndMakeVisible (nextButton);
        nextButton.setButtonText ("Next");
        nextButton.addListener (this);

        addAndMakeVisible (reloadButton);
        reloadButton.setButtonText ("Revert");
        reloadButton.addListener (this);

        addAndMakeVisible (presetMenu);
        presetMenu.setJustificationType (juce::Justification::centred);
        presetMenu.setTextWhenNothingSelected ("");
        presetMenu.onChange = [this] {
            presetMenuChanged();
        };

        loadPresetMenu();
        presetManager.loadPreset (presetManager.getDefaultPresetFile());
        selectCurrentPreset();
    }

    ~PresetManagerView()
    {
        prevButton.removeListener (this);
        nextButton.removeListener (this);
        reloadButton.removeListener (this);
    }

    void paint (juce::Graphics& g) override
    {
    }

    void resized() override
    {
        constexpr int height = 20;
        prevButton.setBounds (getWidth() * 2 / 8, 0, getWidth() / 8, height);
        nextButton.setBounds (getWidth() * 3 / 8, 0, getWidth() / 8, height);
        presetMenu.setBounds (getWidth() * 4 / 8, 0, getWidth() * 3 / 8, height);
        reloadButton.setBounds (getWidth() * 7 / 8, 0, getWidth() / 8, height);
    }

    void buttonClicked (juce::Button* button) override
    {
        if (button == &prevButton)
        {
            prevClicked();
        }

        if (button == &nextButton)
        {
            nextClicked();
        }

        if (button == &reloadButton)
        {
            presetManager.loadPreset (presetManager.getCurrentPresetFile());
        }
    }

private:
    PresetManager presetManager;
    juce::TextButton prevButton;
    juce::TextButton nextButton;
    juce::TextButton reloadButton;

    juce::ComboBox presetMenu;
    juce::PopupMenu factoryPresetMenu;
    juce::PopupMenu userPresetMenu;
    std::unique_ptr<juce::FileChooser> chooser;
    std::unordered_map<juce::File, int, JuceFileHash> itemIdByPreset;
    std::unordered_map<int, juce::File> presetByItemId;
    juce::PopupMenu::Item saveItem; // TODO: remove these member vars?
    juce::PopupMenu::Item saveAsItem;
    juce::PopupMenu::Item goToPresetFolderItem;
    juce::PopupMenu::Item rescanPresetsItem;
    bool doNothingOnPresetMenuChangeCallback;
    //==============================================================================
    /*
    *  It creates something like result of `tree` command on GUI
    */
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
                itemIdByPreset[presets[presetIdx]] = itemId;
                presetByItemId[itemId] = presets[presetIdx];
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
        int itemId = 1; // Item ID should start from 1

        // Default preset
        presetMenu.addItem ("Default", itemId);
        itemIdByPreset[presetManager.getDefaultPresetFile()] = itemId;
        presetByItemId[itemId] = presetManager.getDefaultPresetFile();
        itemId++;

        // Factory presets
        auto factoryPresetFiles = presetManager.getFactoryPresets();
        {
            int i = 0;
            createPresetMenuTree (factoryPresetMenu, itemId, factoryPresetFiles, i, presetManager.getFactoryPresetDir());
        }
        presetMenu.getRootMenu()->addSubMenu ("Factory Presets", factoryPresetMenu);

        // User presets
        auto userPresetFiles = presetManager.getUserPresets();
        {
            int i = 0;
            createPresetMenuTree (userPresetMenu, itemId, userPresetFiles, i, presetManager.getUserPresetDir());
        }
        presetMenu.getRootMenu()->addSubMenu ("User Presets", userPresetMenu);

        // --- Operation menu ---
        // About following callbacks and doNothingOnPresetMenuChangeCallback flag,
        // See presetMenuChanged()'s comment.

        // Separator
        presetMenu.addSeparator();

        // Save
        // TODO: Hide this menu because it's a bit dangerous (It may accidentally save presets
        // by mistake).
        saveItem.itemID = itemId++;
        saveItem.text = "Save";
        saveItem.action = [this]() {
            doNothingOnPresetMenuChangeCallback = true;
            saveClicked();
        };
        presetMenu.getRootMenu()->addItem (saveItem);

        // Save as
        saveAsItem.itemID = itemId++;
        saveAsItem.text = "Save as...";
        saveAsItem.action = [this]() {
            doNothingOnPresetMenuChangeCallback = true;
            saveAsClicked();
        };
        presetMenu.getRootMenu()->addItem (saveAsItem);

        // Go to preset folder
        goToPresetFolderItem.itemID = itemId++;
        goToPresetFolderItem.text = "Go to Preset Folder...";
        goToPresetFolderItem.action = [this]() {
            doNothingOnPresetMenuChangeCallback = true;
            goToPresetFolderClicked();
        };
        presetMenu.getRootMenu()->addItem (goToPresetFolderItem);

        // Rescan presets
        rescanPresetsItem.itemID = itemId++;
        rescanPresetsItem.text = "Rescan Presets";
        rescanPresetsItem.action = [this]() {
            doNothingOnPresetMenuChangeCallback = true;
            rescanPresetsClicked();
        };
        presetMenu.getRootMenu()->addItem (rescanPresetsItem);
    }

    /*
    Callback handler for the preset menu change.

    --- Why this class has multiple callback handlers for the same preset menu combobox? ---
    This class has multiple callback handlers REGISTERED SEPARATELY:

    - presetMenuChanged()       ---- Handles preset change
    - saveClicked()             -| 
    - saveAsClicked()           -|-- Handles operation menu
    - goToPresetFolderClicked() -|
    - goToPresetFolderClicked() -|

    In terms of "clean code" I  wanted to put all of them to presetMenuChanged().
    But there is a problem. When an operation menu is changed, I don't want to
    make the menu label to be changed to the name of the operation like "Save as"
    while juce::Combobox always changes it.
    I tried to find a way to prevent the change, but I couldn't.
    
    The next idea I came up with was to call
    `presetMenu.setSelectedId (previousItemId, juce::dontSendNotification);`
    in the juce::Combobox's callback handler to change the menu label back as soon as possible.
    But it also has a problem. It's very slow and I can see the label is changing
    like "Preset Name" -> "Save as..." -> "Preset Name" on the UI. This is very ugly.

    So my work around is the current implementation.
    juce::PopupMenu::Item's callback handlers are called much earlier than
    juce::Combobox's callback handlers.

    e.g.
    ```
    juce::PopupMenu::Item saveAsItem;
    saveAsItem.action = [this]() {
        presetMenu.setSelectedId (previousItemId, juce::dontSendNotification);
        doSomethingForSaveAsOperation();
    };
    ```
    Even with this workaround, the juce::Combobox's callback handler (presetMenuChanged()) is
    also called after juce::PopupMenu::Item's callback handler.
    So I use `doNothingOnPresetMenuChangeCallback` flag to handle this situation.
    Unfortunately this is also ugly. But this is something that only developers see.
    
    I will try to find a better way in future.
    I guess stop using juce::Combobox for preset menu is an workable option.
    */
    void presetMenuChanged()
    {
        auto selectedItemId = presetMenu.getSelectedId();
        if (doNothingOnPresetMenuChangeCallback)
        {
            // Note that this callback is triggered by operation menu not only by presets.
            // But this is called after operation menu handlers like saveAsClicked().
            // This flag is set only by operation menu handlers.
            doNothingOnPresetMenuChangeCallback = false;
            return;
        }
        // A preset should be selected.
        // Even when this callback is triggered by operation menu,
        // selectedItemId is already changed back by them.
        assert (! isPresetOperationItem (selectedItemId));
        presetManager.loadPreset (presetByItemId[selectedItemId]);
    }

    void prevClicked()
    {
        presetManager.loadPrev();
        selectCurrentPreset();
    }

    void nextClicked()
    {
        presetManager.loadNext();
        selectCurrentPreset();
    }

    void saveClicked()
    {
        selectCurrentPreset();
        presetManager.savePreset (presetManager.getCurrentPresetFile());
    }

    void saveAsClicked()
    {
        selectCurrentPreset();
        chooser = std::make_unique<juce::FileChooser> ("Save as...",
                                                       presetManager.getUserPresetDir(),
                                                       "*.oapreset");
        auto chooserFlag = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
        chooser->launchAsync (chooserFlag, [this] (const juce::FileChooser& chooser) {
            juce::File file (chooser.getResult());
            if (file.getFullPathName() == "")
            {
                selectCurrentPreset();
                return;
            }
            presetManager.savePreset (file);
            presetManager.loadPreset (file);
            loadPresetMenu();
            selectCurrentPreset();
        });
    }

    void goToPresetFolderClicked()
    {
        selectCurrentPreset();
        juce::File dir = presetManager.getUserPresetDir();
        dir.revealToUser();
    }

    void rescanPresetsClicked()
    {
        loadPresetMenu();
        selectCurrentPreset();
    }

    void selectCurrentPreset()
    {
        presetMenu.setSelectedId (itemIdByPreset[presetManager.getCurrentPresetFile()],
                                  juce::NotificationType::dontSendNotification);
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

    bool isPresetOperationItem (int selectedItemId)
    {
        auto presetFiles = presetManager.getPresets();
        int lastPresetItemId = presetMenuItemId (presetFiles.size() - 1);
        /*
        Note that our preset menu looks like...
        ============================
        Default                      <--- ItemID=1
        Factory Presets
        User Presets
        ---------------------------- <--- Operation items are placed below here
        Save
        Save as...
        Go to Preset Folder...
        Rescan Presets
        ============================
        */
        return selectedItemId > lastPresetItemId;
    }
};
} // namespace onsen
