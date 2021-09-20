/*
  ==============================================================================

   Preset Manager

  ==============================================================================
*/

#include "../GlobalVariables.h"
#include <JuceHeader.h>
// #include <iostream>

namespace onsen
{
class PresetManager
{
public:
    PresetManager() : parameters (*GlobalVariables::parameters)
    {
    }

    void save()
    {
        auto state = parameters.copyState();
        std::unique_ptr<juce::XmlElement> stateXml (state.createXml());
        // std::cout << "xml->toString()" << xml->toString() << std::endl;
        juce::File file (juce::File::getSpecialLocation (
                             juce::File::SpecialLocationType::userDesktopDirectory)
                             .getChildFile ("test.preset"));
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

    void load()
    {
        juce::File file (juce::File::getSpecialLocation (
                             juce::File::SpecialLocationType::userDesktopDirectory)
                             .getChildFile ("test.preset"));
        auto xmlDocument = juce::XmlDocument (file);
        std::unique_ptr<juce::XmlElement> presetXml (xmlDocument.getDocumentElement());

        if (presetXml.get() != nullptr)
            if (presetXml->hasTagName ("Preset")
                && presetXml->getChildByName ("State") != nullptr
                && presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()) != nullptr)
                parameters.replaceState (juce::ValueTree::fromXml (
                    *(presetXml->getChildByName ("State")->getChildByName (parameters.state.getType()))));
    }

private:
    juce::AudioProcessorValueTreeState& parameters;
};
} // namespace onsen
