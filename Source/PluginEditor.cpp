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
    setSize (600, 350);

    // Initialize sin oscillator gain slider (knob).
    sSinGain.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sSinGain.setTextBoxIsEditable (false);
    sSinGainLabel.setText (parameters.getParameter ("sinGain")->name, juce::dontSendNotification);
    addAndMakeVisible (sSinGain);
    sSinGainAttachment = std::make_unique<SliderAttachment> (parameters, "sinGain", sSinGain);
    addAndMakeVisible (sSinGainLabel);

    // Initialize square oscillator gain slider (knob).
    sSquareGain.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sSquareGain.setTextBoxIsEditable (false);
    sSquareGainLabel.setText (parameters.getParameter ("squareGain")->name, juce::dontSendNotification);
    addAndMakeVisible (sSquareGain);
    sSquareGainAttachment = std::make_unique<SliderAttachment> (parameters, "squareGain", sSquareGain);
    addAndMakeVisible (sSquareGainLabel);

    // Initialize triangle oscillator gain slider (knob).
    sTriangleGain.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    sTriangleGain.setTextBoxIsEditable (false);
    sTriangleGainLabel.setText (parameters.getParameter ("triangleGain")->name, juce::dontSendNotification);
    addAndMakeVisible (sTriangleGain);
    sTriangleGainAttachment = std::make_unique<SliderAttachment> (parameters, "triangleGain", sTriangleGain);
    addAndMakeVisible (sTriangleGainLabel);

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
    constexpr int deltaX = 150;
    constexpr int deltaY = 120;
    constexpr int sliderW = 150;
    constexpr int sliderH = 100;
    constexpr int sliderTxtW = 80;
    constexpr int sliderTxtH = 20;
    // {label's Y} = {slider's Y} + {this value}
    constexpr int sliderLabelDistY = 58;
    constexpr int labelW = 100;
    constexpr int labelH = 30;

    sSinGain.setBounds (0, 0, sliderW, sliderH);
    sSinGain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sSinGainLabel.setBounds (0, sliderLabelDistY, labelW, labelH);

    sSquareGain.setBounds (deltaX, 0, sliderW, sliderH);
    sSquareGain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sSquareGainLabel.setBounds (deltaX, sliderLabelDistY, labelW, labelH);

    sTriangleGain.setBounds (deltaX * 2, 0, sliderW, sliderH);
    sTriangleGain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sTriangleGainLabel.setBounds (deltaX * 2, sliderLabelDistY, labelW, labelH);

    sAttack.setBounds (0, deltaY, sliderW, sliderH);
    sAttack.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sAttackLabel.setBounds (0, sliderLabelDistY + deltaY, labelW, labelH);

    sDecay.setBounds (deltaX, deltaY, sliderW, sliderH);
    sDecay.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sDecayLabel.setBounds (deltaX, sliderLabelDistY + deltaY, labelW, labelH);

    sSustain.setBounds (deltaX * 2, deltaY, sliderW, sliderH);
    sSustain.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sSustainLabel.setBounds (deltaX * 2, sliderLabelDistY + deltaY, labelW, labelH);

    sRelease.setBounds (deltaX * 3, deltaY, sliderW, sliderH);
    sRelease.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sReleaseLabel.setBounds (deltaX * 3, sliderLabelDistY + deltaY, labelW, labelH);

    sFrequency.setBounds (0, deltaY * 2, sliderW, sliderH);
    sFrequency.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sFrequencyLabel.setBounds (0, sliderLabelDistY + deltaY * 2, labelW, labelH);

    sResonance.setBounds (deltaX, deltaY * 2, sliderW, sliderH);
    sResonance.setTextBoxStyle (juce::Slider::TextBoxBelow, false, sliderTxtW, sliderTxtH);
    sResonanceLabel.setBounds (deltaX, sliderLabelDistY + deltaY * 2, labelW, labelH);
}
