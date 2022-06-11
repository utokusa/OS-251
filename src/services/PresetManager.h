/*
  ==============================================================================

   Preset Manager

  ==============================================================================
*/

#pragma once

#include "../IAudioProcessorState.h"
#include "FactoryPresets.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================

/*
PresetManager

Note: scanPresets() need to be called before save/load presets
*/
class PresetManager
{
public:
    PresetManager (IAudioProcessorState* _processorState, juce::File _presetDir);
    void scanPresets();
    juce::File getDefaultPresetFile();
    juce::File getFactoryPresetDir();
    juce::File getUserPresetDir();
    juce::Array<juce::File> getPresets();
    juce::Array<juce::File> getFactoryPresets();
    juce::Array<juce::File> getUserPresets();

    /*
    Save preset

    Folder should exist before calling it.
    */
    void savePreset (juce::File file);

    /*
    Loads preset file. If the file is not found or valid, loads default
    */
    void loadPreset (juce::File file);

    juce::File getCurrentPresetFile();
    static juce::String getPresetName (juce::File file);
    void loadPrev();
    void loadNext();
    void requireToUpdatePresetNameOnUI();
    static juce::ValueTree fixPresetState (juce::ValueTree& state);
    static juce::ValueTree fixProcessorState (juce::ValueTree& state);

    //==============================================================================
    std::function<void()> onNeedToUpdateUI;

private:
    IAudioProcessorState* processorState;
    const juce::File presetDir;
    juce::Array<juce::File> factoryPresetFiles;
    juce::Array<juce::File> userPresetFiles;
    juce::Array<juce::File> presetFiles;
    juce::File currentPresetFile;
    static constexpr float PARAM_EPSILON = 0.0001;

    //==============================================================================
    bool validatePresetFile (juce::File file);
    bool validatePresetXml (juce::XmlElement const* const presetXml);
    void loadPresetState (juce::XmlElement const* const presetXml);
    void loadDefaultFileSafely();
    juce::File getPresetDir();
    juce::Array<juce::File> scanPresets (juce::File dir, juce::Array<juce::File>& presetFiles);
    void restorePresetFoldersAndPresetsIfNecessary();
    void restoreDefaultPreset();
    void restoreFactoryPresets();
    void restoreUserPresetFolder();
    void updateCurrentPresetBasedOnProcessorState();
    static bool isParamValueValid (const juce::var& value);
    void setPresetNameToProcessorState (const juce::File& presetFile);
};
} // namespace onsen
