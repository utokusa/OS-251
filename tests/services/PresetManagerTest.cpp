/*
  ==============================================================================
   Preset Manager Test
  ==============================================================================
*/

#include "../../src/services/PresetManager.h"
#include "../../src/services/TmpFileManager.h"
#include "AudioProcessorStateMock.h"
#include "PresetsForTest.h"
#include <JuceHeader.h>
#include <gtest/gtest.h>
#include <iostream>
#include <string>

namespace onsen
{
//==============================================================================
class PresetManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean up preset folder
        testDir.deleteRecursively();
    }

    void TearDown() override
    {
        // Clean up preset folder
        testDir.deleteRecursively();
    }

    const juce::File testDir { onsen::TmpFileManager::getTmpDir().getChildFile ("preset_test") };
    const juce::File testPresetDir { testDir.getChildFile ("presets") };
    AudioProcessorStateMock processorState;
    PresetManager presetManager { &processorState, testPresetDir };
};
//==============================================================================

TEST_F (PresetManagerTest, GetDirectories)
{
    EXPECT_EQ (presetManager.getDefaultPresetFile(), testPresetDir.getChildFile ("Default.oapreset"));
    EXPECT_EQ (presetManager.getFactoryPresetDir(), testPresetDir.getChildFile ("Factory"));
    EXPECT_EQ (presetManager.getUserPresetDir(), testPresetDir.getChildFile ("User"));
}

TEST_F (PresetManagerTest, BeforeFirstScan)
{
    // Before scan
    EXPECT_FALSE (testPresetDir.exists());
    EXPECT_EQ (presetManager.getCurrentPresetFile(), juce::String (""));
}

TEST_F (PresetManagerTest, FirstScan)
{
    presetManager.scanPresets();
    EXPECT_TRUE (testPresetDir.exists());
    EXPECT_EQ (presetManager.getCurrentPresetFile(), juce::String (""));
}

TEST_F (PresetManagerTest, LoadDefaultPreset)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check some of the preset values
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "subSquareGain")->getAttributeValue (1 /*value*/), "1.0");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "attack")->getAttributeValue (1), "0.0");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "resonance")->getAttributeValue (1), "0.3499999940395355");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "masterVolume")->getAttributeValue (1), "0.5");
}

TEST_F (PresetManagerTest, LoadPreset)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getFactoryPresetDir().getChildFile ("Bass/Bass0.oapreset"));
    // Now current preset should be Bass0
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getFactoryPresetDir().getChildFile ("Bass/Bass0.oapreset"));
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check a preset values
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "portamento")->getAttributeValue (1 /*value*/), "0.1000000089406967");
}

TEST_F (PresetManagerTest, LoadNonexistingPreset)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getFactoryPresetDir().getChildFile ("NotExist.oapreset"));
    // Now current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getDefaultPresetFile());
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check a preset values just in case
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "portamento")->getAttributeValue (1 /*value*/), "0.0");
}

TEST_F (PresetManagerTest, LoadBrokenPreset)
{
    presetManager.scanPresets();
    auto file = presetManager.getUserPresetDir().getChildFile ("Broken.oapreset");
    juce::FileOutputStream fs (file);
    fs.writeString ("Broken Preset XML");
    fs.flush();

    presetManager.loadPreset (file);
    // Now current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getDefaultPresetFile());
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check a preset values just in case
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "portamento")->getAttributeValue (1 /*value*/), "0.0");
}

TEST_F (PresetManagerTest, LoadBrokenDefaultPreset)
{
    presetManager.scanPresets();
    auto file = presetManager.getDefaultPresetFile();
    juce::FileOutputStream fs (file);
    fs.writeString ("Broken Preset XML");
    fs.flush();

    presetManager.loadPreset (file);
    // Current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getDefaultPresetFile());
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check some of the default values
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "subSquareGain")->getAttributeValue (1 /*value*/), "1.0");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "attack")->getAttributeValue (1), "0.0");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "resonance")->getAttributeValue (1), "0.3499999940395355");
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "masterVolume")->getAttributeValue (1), "0.5");
}

// LoadPresetWithMissingParameter
TEST_F (PresetManagerTest, LoadPresetWithMissingParameter)
{
    presetManager.scanPresets();
    // preset data from version 1.1.0
    auto file = presetManager.getUserPresetDir().getChildFile ("OldPresetVersion110.oapreset");
    juce::FileOutputStream fs (file);
    fs.writeString (oldPresetVersion110);
    fs.flush();

    presetManager.loadPreset (file);
    // Current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check one of the preset's values
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "decay")->getAttributeValue (1 /*value*/), "0.3199999630451202");
    // Check unison's value which is not included in preset for version 1.1.0
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "unisonOn")->getAttributeValue (1 /*value*/), "0.0");
}

TEST_F (PresetManagerTest, LoadPresetWithNonSupportedParameter)
{
    // preset data with an invalid param (Or future version's parameter)
    presetManager.scanPresets();
    auto file = presetManager.getUserPresetDir().getChildFile ("presetWithInvalidParam.oapreset");
    juce::FileOutputStream fs (file);
    fs.writeString (presetWithInvalidParam);
    fs.flush();

    presetManager.loadPreset (file);
    // Current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // Check one of the preset's values
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "decay")->getAttributeValue (1 /*value*/), "0.3199999630451202");
    // Check extra invalid parameter is not loaded
    // "futureParam" is included in the preset file but not current version of OS-251
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "futureParam"), nullptr);
}

TEST_F (PresetManagerTest, LoadPresetWithInvalidParameterValue)
{
    presetManager.scanPresets();
    auto file = presetManager.getUserPresetDir().getChildFile ("presetWithInvalidParamValue.oapreset");
    juce::FileOutputStream fs (file);
    fs.writeString (presetWithInvalidParamValue);
    fs.flush();

    presetManager.loadPreset (file);
    // Current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // "decay" value is "100.0" in the input, so the default value should be used
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "decay")->getAttributeValue (1 /*value*/), "1.0");
}

TEST_F (PresetManagerTest, LoadPresetWithInvalidParameterValueType)
{
    presetManager.scanPresets();
    auto file = presetManager.getUserPresetDir().getChildFile ("presetWithInvalidParamValueType.oapreset");
    juce::FileOutputStream fs (file);
    fs.writeString (presetWithInvalidParamValueType);
    fs.flush();

    presetManager.loadPreset (file);
    // Current preset should be the default
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_TRUE (stateXml->hasTagName ("OS-251"));
    // "decay" value is "invalid" in the input, so the default value should be used
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "decay")->getAttributeValue (1 /*value*/), "1.0");
}

TEST_F (PresetManagerTest, LoadPrevAndNext)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    auto presets = presetManager.getPresets();
    auto firstPreset = presetManager.getDefaultPresetFile();
    auto secondPreset = presetManager.getFactoryPresetDir().getChildFile ("Bass/Bass0.oapreset");
    auto lastPreset = presets[presets.size() - 1];

    // ---------------------------------------------------------
    // Load next
    EXPECT_EQ (presetManager.getCurrentPresetFile(), firstPreset);
    presetManager.loadNext();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), secondPreset);

    // ---------------------------------------------------------
    // Load prev
    presetManager.loadPrev();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), firstPreset);

    // ---------------------------------------------------------
    // Try to load the previous when the first preset is loaded.
    presetManager.loadPrev();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), firstPreset);

    // ---------------------------------------------------------
    // Try to load the previous when the first preset is loaded and edited.
    // (Load should not happen)
    auto statePtr = processorState.getState();
    // std::cout << "attack before" << statePtr->getChild (0).toXmlString() << std::endl;
    // std::cout << "attack value:" << statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString() << std::endl;
    statePtr->getChild (1 /*attack*/).setProperty (juce::Identifier ("value"), "0.123", nullptr);
    // std::cout << "attack after" << statePtr->getChild (0).toXmlString() << std::endl;
    presetManager.loadPrev();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), firstPreset);

    // Check attack value is not changed
    auto state = processorState.copyState();
    auto stateXml = state.createXml();
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "attack")->getAttributeValue (1 /*value*/).toStdString(), "0.123");

    // ---------------------------------------------------------
    // Try to load the next when the last preset is loaded.
    presetManager.loadPreset (lastPreset);
    presetManager.loadNext();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), lastPreset);

    // ---------------------------------------------------------
    // Try to load the next when the last preset is loaded and edited.
    // (Load should not happen)
    statePtr->getChild (1 /*attack*/).setProperty (juce::Identifier ("value"), "0.123" /*original value should not be this value*/, nullptr);
    presetManager.loadNext();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), lastPreset);

    // Check attack value is not changed
    state = processorState.copyState();
    stateXml = state.createXml();
    EXPECT_EQ (stateXml->getChildByAttribute ("id", "attack")->getAttributeValue (1 /*value*/).toStdString(), "0.123");
}

TEST_F (PresetManagerTest, SavePreset)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());

    auto statePtr = processorState.getState();
    statePtr->getChild (1 /*attack*/).setProperty (juce::Identifier ("value"), "0.123", nullptr);

    // "Save as..."
    auto newPresetFile = presetManager.getUserPresetDir().getChildFile ("Test.oapreset");
    presetManager.savePreset (newPresetFile);
    EXPECT_EQ (presetManager.getCurrentPresetFile(), newPresetFile);

    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    EXPECT_EQ (statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString(), "0.0");

    presetManager.loadPreset (newPresetFile);
    EXPECT_EQ (statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString(), "0.123");

    // "Save"
    statePtr->getChild (1 /*attack*/).setProperty (juce::Identifier ("value"), "0.456", nullptr);
    presetManager.savePreset (newPresetFile);
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    EXPECT_EQ (statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString(), "0.0");
    presetManager.loadPreset (newPresetFile);
    EXPECT_EQ (statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString(), "0.456");
}

TEST_F (PresetManagerTest, SavePresetUsingPathWithoutExtension)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());

    // If the .oapreset extension is not provided, it should add the extension.
    presetManager.savePreset (presetManager.getUserPresetDir().getChildFile ("without_extension"));
    EXPECT_EQ (presetManager.getCurrentPresetFile(),
               presetManager.getUserPresetDir().getChildFile ("without_extension.oapreset"));
}

TEST_F (PresetManagerTest, SavePresetUsingPathWithWrongExtension)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());

    // If wrong extension is not provided, it should add the proper extension.
    presetManager.savePreset (presetManager.getUserPresetDir().getChildFile ("file.wrong"));
    EXPECT_EQ (presetManager.getCurrentPresetFile(),
               presetManager.getUserPresetDir().getChildFile ("file.wrong.oapreset"));

    // If wrong extension is not provided, it should add the proper extension.
    presetManager.savePreset (presetManager.getUserPresetDir().getChildFile ("file.wrong.wrong"));
    EXPECT_EQ (presetManager.getCurrentPresetFile(),
               presetManager.getUserPresetDir().getChildFile ("file.wrong.wrong.oapreset"));
}

TEST_F (PresetManagerTest, SaveOutsideOfPresetFolder)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());

    auto file = presetManager.getDefaultPresetFile().getParentDirectory().getChildFile ("outsider.oapreset");
    presetManager.savePreset (file);
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    presetManager.loadNext();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getDefaultPresetFile());

    presetManager.savePreset (file);
    EXPECT_EQ (presetManager.getCurrentPresetFile(), file);
    presetManager.loadPrev();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), presetManager.getDefaultPresetFile());
}

TEST_F (PresetManagerTest, CheckSavedPresetFormat)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    int initialNumPresets = presetManager.getPresets().size();
    auto file = presetManager.getUserPresetDir().getChildFile ("Test0.oapreset");
    presetManager.savePreset (file);

    // Other element should be checked when we load preset anywas.
    // So we only check the actual "SavedByVersion" value here.
    juce::XmlDocument xmlDocument (file);
    std::unique_ptr<juce::XmlElement> presetXml (xmlDocument.getDocumentElement());
    EXPECT_EQ (presetXml->getChildByName ("SavedByVersion")
                   ->getFirstChildElement()
                   ->getText(),
               OS_251_PROJECT_VERSION);
}

TEST_F (PresetManagerTest, TrackPresetsUsingRescan)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
    int initialNumPresets = presetManager.getPresets().size();
    auto test0 = presetManager.getUserPresetDir().getChildFile ("Test0.oapreset");
    auto test1 = presetManager.getUserPresetDir().getChildFile ("Test1.oapreset");
    presetManager.savePreset (test0);
    presetManager.savePreset (test1);
    presetManager.scanPresets(); // TODO: Make it not mandatory after saving presets?
    // This is not the concern of this test... but make sure the number of presets is correct
    // just in case.
    EXPECT_EQ (presetManager.getPresets().size(), initialNumPresets + 2);

    // Delete new presets
    test0.deleteFile();
    test1.deleteFile();
    presetManager.scanPresets();
    EXPECT_EQ (presetManager.getPresets().size(), initialNumPresets);

    // Ignore files with wrong extention
    auto test2 = presetManager.getUserPresetDir().getChildFile ("Test2.wrongextention");
    presetManager.savePreset (test2);
    EXPECT_EQ (presetManager.getPresets().size(), initialNumPresets);
}

TEST_F (PresetManagerTest, RescanPresetAfterDeletingCurrentPreset)
{
    presetManager.scanPresets();
    presetManager.loadPreset (presetManager.getDefaultPresetFile());

    // Create Test0 preset
    auto statePtr = processorState.getState();
    statePtr->getChild (1 /*attack*/).setProperty (juce::Identifier ("value"), "0.123", nullptr);
    auto test0 = presetManager.getUserPresetDir().getChildFile ("Test0.oapreset");
    presetManager.savePreset (test0);
    presetManager.scanPresets(); // TODO: Make it not mandatory after saving presets?

    int numPresets = presetManager.getPresets().size();
    test0.deleteFile();

    // Even after scan, we keep the value as it is
    presetManager.scanPresets();
    EXPECT_EQ (presetManager.getCurrentPresetFile(), juce::String (""));
    EXPECT_EQ (statePtr->getChild (1 /*attack*/).getProperty (juce::Identifier ("value")).toString().toStdString(), "0.123");
    EXPECT_EQ (presetManager.getPresets().size(), numPresets - 1);
}

TEST_F (PresetManagerTest, RescanPresetAfterDeletingDefaultPreset)
{
    presetManager.scanPresets();
    presetManager.loadNext();
    int numPresets = presetManager.getPresets().size();

    // Confirm default preset is restored after deletion.
    auto file = presetManager.getDefaultPresetFile();
    file.deleteFile();
    EXPECT_FALSE (file.existsAsFile());
    presetManager.scanPresets();
    EXPECT_TRUE (file.existsAsFile());
    EXPECT_EQ (presetManager.getPresets().size(), numPresets);
}

TEST_F (PresetManagerTest, RescanPresetAfterDeletingFactoryPresetDir)
{
    presetManager.scanPresets();
    presetManager.loadNext();
    int numPresets = presetManager.getPresets().size();

    // Confirm factory preseta are restored after deletion.
    auto dir = presetManager.getFactoryPresetDir();
    dir.deleteRecursively();
    EXPECT_FALSE (dir.exists());
    presetManager.scanPresets();
    EXPECT_TRUE (dir.exists());
    EXPECT_EQ (presetManager.getPresets().size(), numPresets);
}

TEST_F (PresetManagerTest, RescanPresetAfterDeletingUserPresetDir)
{
    presetManager.scanPresets();
    presetManager.loadNext();
    int numPresets = presetManager.getPresets().size();

    // Confirm user preset folder is restored after deletion.
    auto dir = presetManager.getUserPresetDir();
    dir.deleteRecursively();
    EXPECT_FALSE (dir.exists());
    presetManager.scanPresets();
    EXPECT_TRUE (dir.exists());
}

TEST_F (PresetManagerTest, RescanPresetAfterDeletingPresetDir)
{
    presetManager.scanPresets();
    presetManager.loadNext();
    int numPresets = presetManager.getPresets().size();

    // Confirm preset folder is restored after deletion.
    auto dir = presetManager.getFactoryPresetDir();
    dir.deleteRecursively();
    EXPECT_FALSE (dir.exists());
    presetManager.scanPresets();
    EXPECT_TRUE (dir.exists());
    EXPECT_EQ (presetManager.getPresets().size(), numPresets);
}

} // namespace onsen
