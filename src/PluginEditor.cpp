/*
  ==============================================================================
   JUCE Plugin Editor
  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include <iostream>

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& proc, juce::AudioProcessorValueTreeState& vts, onsen::PresetManager& _presetManager, onsen::ISynthUi* _synthUi)
    : juce::AudioProcessorEditor (&proc),
      audioProcessor (proc),
      presetManager (_presetManager),
      synthUi (_synthUi),
      valueTreeState (vts),
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

    addAndMakeVisible (presetManagerView);

    // Initialize frequency slider (knob).
    // s_freq.setLookAndFeel (&largeKnobLookAndFeel);
    s_freq.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    s_freq.setTextBoxIsEditable (false);
    s_freqLabel.setText (valueTreeState.getParameter ("frequency")->name, juce::dontSendNotification);
    addAndMakeVisible (s_freq);
    s_freqAttachment.reset (new SliderAttachment (valueTreeState, "frequency", s_freq));
    addAndMakeVisible (s_freqLabel);

    setSize (600, 400);
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
    presetManagerView.setBounds (20, 20, 600 - 20, 30);

    s_freq.setBounds (20, 80, 100, 100);
    s_freq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 30);
    s_freqLabel.setBounds (20, 60, 100, 30);
}

void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (onsen::colors::darkGray2));
}