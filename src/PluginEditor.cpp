/*
  ==============================================================================
    This file contains the basic framework code for a JUCE plugin editor.
  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "views/Body.h"

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& p, juce::AudioProcessorValueTreeState& params)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      parameters (params),
      appRoot(),
      harness(),
      tmpUiBuldlePath()
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    const juce::File dir = juce::File::getSpecialLocation (juce::File::tempDirectory);
    const juce::String jsFileName = "main.js";
    juce::File bundle = dir.createTempFile (jsFileName);
    tmpUiBuldlePath = bundle.getFullPathName(); // It will be deleted in destructor

    {
        juce::FileOutputStream fs = juce::FileOutputStream (bundle);
        fs.write (BinaryData::main_js, BinaryData::main_jsSize);
        fs.flush();
    }

#if JUCE_DEBUG
    juce::File sourceDir = File (OS251_SOURCE_DIR);
    bundle = sourceDir.getChildFile ("jsui/build/js/main.js");
#endif
    addAndMakeVisible (appRoot);
    appRoot.registerViewType ("BodyView", []() -> reactjuce::ViewManager::ViewPair {
        auto view = std::make_unique<BodyView>();
        auto shadowView = std::make_unique<reactjuce::ShadowView> (view.get());

        return { std::move (view), std::move (shadowView) };
    });

    harness = std::make_unique<reactjuce::AppHarness> (appRoot);
    harness->watch (bundle);

#if JUCE_DEBUG
    harness->start();
#else
    harness->once();
#endif

    setSize (appWidth, appHeight);
}

Os251AudioProcessorEditor::~Os251AudioProcessorEditor()
{
    juce::File bundle = juce::File (tmpUiBuldlePath);
    bundle.deleteFile();
};

//==============================================================================
void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    Image backgroundImage = ImageCache::getFromMemory (BinaryData::background_png, BinaryData::background_pngSize);
    g.drawImageWithin (backgroundImage, 0, 0, bodyWidth, bodyHeight, RectanglePlacement::Flags::xLeft, false);
}

void Os251AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    appRoot.setBounds (getLocalBounds());
}