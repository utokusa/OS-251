/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& p, juce::AudioProcessorValueTreeState& params)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      parameters(params)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    // Initialize attack slider (knob).
    sAttack.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sAttack.setTextBoxIsEditable(false);
    sAttackLabel.setText (parameters.getParameter ("attack")->name, juce::dontSendNotification);
    addAndMakeVisible (sAttack);
    sAttackAttachment.reset (new SliderAttachment (parameters, "attack", sAttack));
    addAndMakeVisible (sAttackLabel);
    
    // Initialize release slider (knob).
    sRelease.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sRelease.setTextBoxIsEditable(false);
    sReleaseLabel.setText (parameters.getParameter ("release")->name, juce::dontSendNotification);
    addAndMakeVisible (sRelease);
    sReleaseAttachment.reset (new SliderAttachment (parameters, "release", sRelease));
    addAndMakeVisible (sReleaseLabel);
}

Os251AudioProcessorEditor::~Os251AudioProcessorEditor()
{
}

//==============================================================================
void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Make sure the build: 1", getLocalBounds(), juce::Justification::centred, 1);
}

void Os251AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    sAttack.setBounds (0, 0, 150, 100);
    sAttack.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    sAttackLabel.setBounds (0,60,100,30);
    
    sRelease.setBounds (200, 0, 150, 100);
    sRelease.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    sReleaseLabel.setBounds (200,60,100,30);
    
}
