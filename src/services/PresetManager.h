/*
  ==============================================================================

   Preset Manager

  ==============================================================================
*/

#pragma once

#include "../GlobalVariables.h"
#include <JuceHeader.h>
// #include <iostream>

namespace onsen
{
class PresetManager
{
public:
    PresetManager() : parameters (*GlobalVariables::parameters),
                      presetFiles(),
                      currentPresetFile(),
                      currentPresetIndex (-1)
    {
        scanUserPresets();
    }

    void save()
    {
        auto state = parameters.copyState();
        std::unique_ptr<juce::XmlElement> stateXml (state.createXml());
        // std::cout << "xml->toString()" << xml->toString() << std::endl;
        juce::File file (juce::File::getSpecialLocation (
                             juce::File::SpecialLocationType::userDesktopDirectory)
                             .getChildFile ("test.oapreset"));
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

    juce::String load()
    {
        juce::File file (juce::File::getSpecialLocation (
                             juce::File::SpecialLocationType::userDesktopDirectory)
                             .getChildFile ("test.oapreset"));
        loadPreset (file);
        return getPresetName (currentPresetFile);
    }

    juce::String loadDefaultPreset()
    {
        // TODO: Stop using actual file to prevent it from being deleted.
        loadPreset (juce::File::getSpecialLocation (
                        juce::File::SpecialLocationType::userApplicationDataDirectory)
                        .getChildFile ("Onsen Audio/OS-251/presets/default.oapreset"));
        return getPresetName (currentPresetFile);
    }

    juce::Array<juce::File> scanUserPresets()
    {
        // File::findChildFiles
        // RangedDirectoryIterator
        juce::File file (juce::File::getSpecialLocation (
                             juce::File::SpecialLocationType::userApplicationDataDirectory)
                             .getChildFile ("Onsen Audio/OS-251/presets/user"));
        file.createDirectory(); // OK if it exists.
        // file.revealToUser();
        auto files = file.findChildFiles (juce::File::TypesOfFileToFind::findFilesAndDirectories + juce::File::TypesOfFileToFind::ignoreHiddenFiles, true, "*.oapreset");
        files.sort();
        std::cout << "User presets" << std::endl;
        for (auto f : files)
        {
            std::cout << f.getFullPathName() << std::endl;
        }

        return presetFiles = files;
    }

    juce::Array<juce::File> getUserPresets()
    {
        return presetFiles;
    }

    void loadPreset (juce::File file)
    {
        currentPresetFile = file;
        std::cout << "loading: " << file.getFullPathName() << std::endl;
        auto xmlDocument = juce::XmlDocument (file);
        std::unique_ptr<juce::XmlElement> presetXml (xmlDocument.getDocumentElement());

        if (presetXml.get() != nullptr)
            if (presetXml->hasTagName ("Preset")
                && presetXml->getChildByName ("State") != nullptr
                && presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()) != nullptr)
                parameters.replaceState (juce::ValueTree::fromXml (
                    *(presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()))));
    }

    static juce::String getPresetName (juce::File file)
    {
        return file.getFileNameWithoutExtension();
    }

private:
    juce::AudioProcessorValueTreeState& parameters;
    juce::Array<juce::File> presetFiles;
    juce::File currentPresetFile;
    int currentPresetIndex;
};
} // namespace onsen
