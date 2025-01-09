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

#include "../IAudioProcessorState.h"
#include "../services/PresetManager.h"
#include "PresetMenuLookAndFeel.h"
#include <JuceHeader.h>
#include <cassert>
#include <functional>
#include <string>
#include <unordered_map>

namespace onsen
{
//==============================================================================
class PresetManagerView : public juce::Component, public juce::Button::Listener
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
    PresetManagerView (PresetManager&);
    ~PresetManagerView();
    void resized() override;
    void buttonClicked (juce::Button* button) override;

private:
    PresetManager& presetManager;
    PresetMenuLookAndFeel presetMenuLookAndFeel;
    juce::Image prevButtonImage;
    juce::Image prevButtonOverImage;
    juce::Image prevButtonDownImage;
    juce::ImageButton prevButton;
    juce::Image nextButtonImage;
    juce::Image nextButtonOverImage;
    juce::Image nextButtonDownImage;
    juce::ImageButton nextButton;
    juce::Image reloadButtonImage;
    juce::Image reloadButtonOverImage;
    juce::Image reloadButtonDownImage;
    juce::ImageButton reloadButton;
    juce::ComboBox presetMenu;
    juce::PopupMenu factoryPresetMenu;
    juce::PopupMenu userPresetMenu;
    std::unique_ptr<juce::FileChooser> chooser;
    std::unordered_map<juce::File, int, JuceFileHash> itemIdByPreset;
    std::unordered_map<int, juce::File> presetByItemId;
    juce::PopupMenu::Item saveItem;
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
                               juce::File currentDir);

    void loadPresetMenu();
    void presetMenuChanged();
    void prevClicked();
    void nextClicked();
    void saveClicked();
    void saveAsClicked();
    void goToPresetFolderClicked();
    void rescanPresetsClicked();
    void selectCurrentPreset();
    int presetArrayIdx (int presetMenuItemId);
    int presetMenuItemId (int presetArrayIdx);
    bool isPresetOperationItem (int selectedItemId);
};
} // namespace onsen
