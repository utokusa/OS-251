/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& p, juce::AudioProcessorValueTreeState& params)
    : AudioProcessorEditor (&p),
      audioProcessor (p),
      parameters (params)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 500);

    // Initialize attack slider (knob).
    sAttack.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sAttack.setTextBoxIsEditable (false);
    sAttackLabel.setText (parameters.getParameter ("attack")->name, juce::dontSendNotification);
    addAndMakeVisible (sAttack);
    sAttackAttachment = std::make_unique<SliderAttachment> (parameters, "attack", sAttack);
    addAndMakeVisible (sAttackLabel);

    // Initialize decay slider (knob).
    sDecay.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sDecay.setTextBoxIsEditable (false);
    sDecayLabel.setText (parameters.getParameter ("decay")->name, juce::dontSendNotification);
    addAndMakeVisible (sDecay);
    sDecayAttachment = std::make_unique<SliderAttachment> (parameters, "decay", sDecay);
    addAndMakeVisible (sDecayLabel);

    // Initialize sustain slider (knob).
    sSustain.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sSustain.setTextBoxIsEditable (false);
    sSustainLabel.setText (parameters.getParameter ("sustain")->name, juce::dontSendNotification);
    addAndMakeVisible (sSustain);
    sSustainAttachment = std::make_unique<SliderAttachment> (parameters, "sustain", sSustain);
    addAndMakeVisible (sSustainLabel);

    // Initialize release slider (knob).
    sRelease.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sRelease.setTextBoxIsEditable (false);
    sReleaseLabel.setText (parameters.getParameter ("release")->name, juce::dontSendNotification);
    addAndMakeVisible (sRelease);
    sReleaseAttachment = std::make_unique<SliderAttachment> (parameters, "release", sRelease);
    addAndMakeVisible (sReleaseLabel);

    // Initialize filter frequency slider (knob).
    sFrequency.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sFrequency.setTextBoxIsEditable (false);
    sFrequencyLabel.setText (parameters.getParameter ("frequency")->name, juce::dontSendNotification);
    addAndMakeVisible (sFrequency);
    sFrequencyAttachment = std::make_unique<SliderAttachment> (parameters, "frequency", sFrequency);
    addAndMakeVisible (sFrequencyLabel);

    // Initializer filter resonance slider (knob).
    sResonance.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sResonance.setTextBoxIsEditable (false);
    sResonanceLabel.setText (parameters.getParameter ("resonance")->name, juce::dontSendNotification);
    addAndMakeVisible (sResonance);
    sResonanceAttachment = std::make_unique<SliderAttachment> (parameters, "resonance", sResonance);
    addAndMakeVisible (sResonanceLabel);
}

Os251AudioProcessorEditor::~Os251AudioProcessorEditor() = default;

//==============================================================================
void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // For debugging
#ifdef DEBUG
    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Build: " __DATE__ " " __TIME__, getLocalBounds(), juce::Justification::bottomRight, 1);
#endif
}

void Os251AudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    // Lay out parameters
    constexpr int deltaX = 200;
    constexpr int deltaY = 150;
    constexpr int sliderW = 150;
    constexpr int sliderH = 100;
    constexpr int sliderTxtW = 80;
    constexpr int sliderTxtH = 20;
    // {label's Y} = {slider's Y} + {this value}
    constexpr int sliderLabelDistY = 58;
    constexpr int labelW = 100;
    constexpr int labelH = 30;

    sAttack.setBounds (0, 0, sliderW, sliderH);
    sAttack.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sAttackLabel.setBounds (0, sliderLabelDistY, labelW, labelH);

    sDecay.setBounds (deltaX, 0, sliderW, sliderH);
    sDecay.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sDecayLabel.setBounds (deltaX, sliderLabelDistY, labelW, labelH);

    sSustain.setBounds (0, deltaY, sliderW, sliderH);
    sSustain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sSustainLabel.setBounds (0, sliderLabelDistY + deltaY, labelW, labelH);

    sRelease.setBounds (deltaX, deltaY, sliderW, sliderH);
    sRelease.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sReleaseLabel.setBounds (deltaX, sliderLabelDistY + deltaY, labelW, labelH);

    sFrequency.setBounds (0, deltaY * 2, sliderW, sliderH);
    sFrequency.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sFrequencyLabel.setBounds (0, sliderLabelDistY + deltaY * 2, labelW, labelH);

    sResonance.setBounds (deltaX, deltaY * 2, sliderW, sliderH);
    sResonance.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sResonanceLabel.setBounds (deltaX, sliderLabelDistY + deltaY * 2, labelW, labelH);
}
