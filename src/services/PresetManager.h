/*
  ==============================================================================

   Preset Manager

  ==============================================================================
*/

#pragma once

#include "../GlobalVariables.h"
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
    PresetManager() : parameters (*GlobalVariables::parameters),
                      factoryPresetFiles(),
                      userPresetFiles(),
                      presetFiles(),
                      currentPresetFile (getDefaultPresetFile())
    {
    }

    void scanPresets()
    {
        restorePresetFoldersAndPresetsIfNecessary();
        scanPresets (getFactoryPresetDir(), factoryPresetFiles);
        scanPresets (getUserPresetDir(), userPresetFiles);
        presetFiles.clear();
        presetFiles.add (getDefaultPresetFile());
        presetFiles.addArray (factoryPresetFiles);
        presetFiles.addArray (userPresetFiles);
        if (! validatePresetFile (currentPresetFile))
        {
            currentPresetFile = getDefaultPresetFile();
        }
    }

    juce::File getDefaultPresetFile()
    {
        return getPresetDir().getChildFile ("Default.oapreset");
    }

    juce::File getFactoryPresetDir()
    {
        return getPresetDir().getChildFile ("Factory");
    }

    juce::File getUserPresetDir()
    {
        return getPresetDir().getChildFile ("User");
    }

    juce::Array<juce::File> getPresets()
    {
        return presetFiles;
    }

    juce::Array<juce::File> getFactoryPresets()
    {
        return factoryPresetFiles;
    }

    juce::Array<juce::File> getUserPresets()
    {
        return userPresetFiles;
    }

    void savePreset (juce::File file)
    {
        auto state = parameters.copyState();
        std::unique_ptr<juce::XmlElement> stateXml (state.createXml());

        auto presetXml = std::make_unique<juce::XmlElement> ("Preset");

        auto gadget = new juce::XmlElement ("Gadget");
        gadget->addTextElement ("OS-251");
        presetXml->addChildElement (gadget);

        auto savedByVersion = new juce::XmlElement ("SavedByVersion");
        savedByVersion->addTextElement ("1.0.1"); // TODO: implement
        presetXml->addChildElement (savedByVersion);

        auto version = new juce::XmlElement ("Version");
        version->addTextElement ("0");
        presetXml->addChildElement (version);

        auto stateContainerXml = std::make_unique<juce::XmlElement> ("State");
        stateContainerXml->addChildElement (stateXml.release());
        presetXml->addChildElement (stateContainerXml.release());
        presetXml->writeTo (file);
    }

    /*
    Loads preset file. If the file is not found or valid, loads default
    */
    void loadPreset (juce::File file)
    {
        // TODO: If the file is not found, load default instead

        std::cout << "loading: " << file.getFullPathName() << std::endl;
        auto xmlDocument = juce::XmlDocument (file);
        std::unique_ptr<juce::XmlElement> presetXml (xmlDocument.getDocumentElement());

        if (validatePresetXml (presetXml.get()))
            parameters.replaceState (juce::ValueTree::fromXml (
                *(presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()))));

        currentPresetFile = file;
    }

    juce::File getCurrentPresetFile()
    {
        return currentPresetFile;
    }

    static juce::String getPresetName (juce::File file)
    {
        return file.getFileNameWithoutExtension();
    }

    void loadPrev()
    {
        // TODO: Use hash table or binary search. Or cache
        //       currentPresetFile's index (Note that cached index can't be used
        //       after presetFiles is updated)
        int idx = presetFiles.indexOf (currentPresetFile);
        if (idx > 0)
            loadPreset (presetFiles[idx - 1]);
        else
            loadPreset (getDefaultPresetFile());
    }

    void loadNext()
    {
        int idx = presetFiles.indexOf (currentPresetFile);
        if (idx < presetFiles.size() - 1)
            loadPreset (presetFiles[idx + 1]);
        else
            loadPreset (getDefaultPresetFile());
    }

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::Array<juce::File> factoryPresetFiles;
    juce::Array<juce::File> userPresetFiles;
    juce::Array<juce::File> presetFiles;
    juce::File currentPresetFile;

    //==============================================================================
    bool validatePresetFile (juce::File file)
    {
        if (file.getFullPathName() == "" || ! file.existsAsFile())
            return false;
        auto xmlDocument = juce::XmlDocument (file);
        std::unique_ptr<juce::XmlElement> presetXml (xmlDocument.getDocumentElement());

        return validatePresetXml (presetXml.get());
    }

    bool validatePresetXml (juce::XmlElement* presetXml)
    {
        if (presetXml != nullptr
            && presetXml->hasTagName ("Preset")
            && presetXml->getChildByName ("State") != nullptr
            && presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()) != nullptr)
            return true;

        return false;
    }

    juce::File getPresetDir()
    {
        return juce::File::getSpecialLocation (
                   juce::File::SpecialLocationType::userApplicationDataDirectory)
            .getChildFile ("Onsen Audio/OS-251/presets");
    }

    juce::Array<juce::File> scanPresets (juce::File dir, juce::Array<juce::File>& presetFiles)
    {
        dir.createDirectory(); // OK if it exists.
        auto files = dir.findChildFiles (juce::File::TypesOfFileToFind::findFiles, true, "*.oapreset");
        files.sort();

        return presetFiles = files;
    }

    void restorePresetFoldersAndPresetsIfNecessary()
    {
        // Restore default preset if it doesn't exist.
        if (! getDefaultPresetFile().existsAsFile())
        {
            restoreDefaultPreset();
        }

        // Restore factory preset folder if it doesn't exist.
        if (! getFactoryPresetDir().exists() || getFactoryPresetDir().existsAsFile())
        {
            restoreFactoryPresets();
        }

        // Restore user preset folder if it doesn't exist.
        if (! getUserPresetDir().exists())
        {
            restoreUserPresetFolder();
        }
    }

    void restoreDefaultPreset()
    {
        auto file = getDefaultPresetFile();

        // Try to delete a folder with the same name as default preset file
        file.deleteRecursively();
        // This line will create necessary sub folders
        file.create();
        auto fs = juce::FileOutputStream (file);
        fs.write (BinaryData::Default_oapreset, BinaryData::Default_oapresetSize);
        fs.flush();
    }

    void restoreFactoryPresets()
    {
        // Try to delete a file with the same name as factory preset folder
        // (Usually it doesn't happen).
        getFactoryPresetDir().deleteFile();

        auto dir = getFactoryPresetDir();
        for (auto& factoryPreset : factoryPresets)
        {
            auto dir = getFactoryPresetDir();
            auto file = dir.getChildFile (factoryPreset.path);
            // This line will create necessary sub folders
            file.create();
            auto fs = juce::FileOutputStream (file);
            fs.write (factoryPreset.data, factoryPreset.size);
            fs.flush();
        }
    }

    void restoreUserPresetFolder()
    {
        auto dir = getUserPresetDir();
        if (dir.exists())
            return;
        dir.createDirectory();
    }
};
} // namespace onsen