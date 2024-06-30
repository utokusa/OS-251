/*
  ==============================================================================
   JUCE Plugin Editor
  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include <iostream>

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& proc, juce::AudioProcessorValueTreeState&, onsen::PresetManager& _presetManager, onsen::ISynthUi* _synthUi)
    : juce::AudioProcessorEditor (&proc),
      audioProcessor (proc),
      presetManager (_presetManager),
      synthUi (_synthUi),
      genericEditor (audioProcessor),
      presetManagerView (presetManager)
{
    auto& processorParams = audioProcessor.getParameters();

    for (auto& param : processorParams)
    {
        auto paramWithId = dynamic_cast<juce::AudioProcessorParameterWithID*> (param);
        assert (paramWithId);
        parameterById[paramWithId->paramID] = param;
        param->addListener (this);
    }

    addAndMakeVisible (genericEditor);
    addAndMakeVisible (presetManagerView);

    setSize (genericEditor.getWidth(), genericEditor.getHeight() + 45);
    startTimerHz (30);
}

Os251AudioProcessorEditor::~Os251AudioProcessorEditor()
{
    for (auto& param : audioProcessor.getParameters())
    {
        param->removeListener (this);
    }
};

//==============================================================================
void Os251AudioProcessorEditor::parameterValueChanged (int parameterIndex, float newValue)
{
}

void Os251AudioProcessorEditor::parameterGestureChanged (int, bool)
{
}

//==============================================================================
void Os251AudioProcessorEditor::timerCallback()
{
}

//==============================================================================
void Os251AudioProcessorEditor::resized()
{
    presetManagerView.setBounds (20, getHeight() - 30, genericEditor.getWidth() - 20, 30);
}

void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (onsen::colors::darkGray2));
}