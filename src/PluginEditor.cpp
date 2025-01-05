/*
  ==============================================================================
   JUCE Plugin Editor
  ==============================================================================
*/

#include "PluginEditor.h"
#include "PluginProcessor.h"
#include "views/colors.h"
#include <iostream>

//==============================================================================
Os251AudioProcessorEditor::Os251AudioProcessorEditor (Os251AudioProcessor& proc, juce::AudioProcessorValueTreeState& vts, onsen::PresetManager& _presetManager, onsen::ISynthUi* _synthUi)
    : juce::AudioProcessorEditor (&proc),
      audioProcessor (proc),
      presetManager (_presetManager),
      synthUi (_synthUi),
      valueTreeState (vts),
      presetManagerView (presetManager),
      clippingIndicatorView (_synthUi)
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

    addAndMakeVisible (clippingIndicatorView);

    // Initialize frequency slider (knob).
    // // s_freq.setLookAndFeel (&largeKnobLookAndFeel);
    // s_freq.setSliderStyle (juce::Slider::RotaryVerticalDrag);
    // s_freq.setTextBoxIsEditable (false);
    // s_freqLabel.setText (valueTreeState.getParameter ("frequency")->name, juce::dontSendNotification);
    // addAndMakeVisible (s_freq);
    // s_freqAttachment.reset (new SliderAttachment (valueTreeState, "frequency", s_freq));
    // addAndMakeVisible (s_freqLabel);

    // // b_chorusOn.setLookAndFeel (&largeKnobLookAndFeel);
    // // b_chorusOn.setColour (juce::TextButton::buttonColourId, juce::Colours::red);
    // b_chorusOn.setColour (juce::TextButton::buttonOnColourId, juce::Colours::green);
    // b_chorusOnLabel.setText (valueTreeState.getParameter ("chorusOn")->name, juce::dontSendNotification);
    // addAndMakeVisible (b_chorusOn);
    // b_chorusOnAttachment.reset (new ButtonAttachment (valueTreeState, "chorusOn", b_chorusOn));

    // b_chorusOn.setClickingTogglesState (true);
    // addAndMakeVisible (b_chorusOnLabel);

    for (const auto& p : paramLayout)
    {
        const auto& paramName = p.first;
        if (p.second == ParamType::SLIDER)
        {
            sliders.try_emplace (paramName, std::make_unique<juce::Slider> (paramName));
            sliderLabels.try_emplace (paramName, std::make_unique<juce::Label>());
            sliderAttachment.try_emplace (paramName, std::make_unique<SliderAttachment> (valueTreeState, paramName, *sliders[paramName]));

            // sliders[paramName]->setLookAndFeel (&largeKnobLookAndFeel);
            sliders[paramName]->setSliderStyle (juce::Slider::RotaryVerticalDrag);
            sliders[paramName]->setTextBoxIsEditable (false);
            sliderLabels[paramName]->setText (valueTreeState.getParameter (paramName)->name, juce::dontSendNotification);
            addAndMakeVisible (*sliders[paramName]);
            addAndMakeVisible (*sliderLabels[paramName]);
        }
        else /*(p.second == ParamType::BUTTON)*/
        {
            buttons.try_emplace (paramName, std::make_unique<juce::TextButton> (paramName));
            buttonLabels.try_emplace (paramName, std::make_unique<juce::Label>());
            buttonAttachment.try_emplace (paramName, std::make_unique<ButtonAttachment> (valueTreeState, paramName, *buttons[paramName]));

            // buttons[paramName]->setLookAndFeel (&largeKnobLookAndFeel);
            // buttons[paramName]->setColour (juce::TextButton::buttonColourId, juce::Colours::red);
            buttons[paramName]->setColour (juce::TextButton::buttonOnColourId, juce::Colours::green);
            buttons[paramName]->setButtonText ("");
            buttonLabels[paramName]->setText (valueTreeState.getParameter (paramName)->name, juce::dontSendNotification);
            addAndMakeVisible (*buttons[paramName]);

            buttons[paramName]->setClickingTogglesState (true);
            addAndMakeVisible (*buttonLabels[paramName]);
        }
    }

    setSize (840, 560);
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
    constexpr int clippingIndicatorViewWidth = 100;
    presetManagerView.setBounds (20, 20, 600 - 20 - clippingIndicatorViewWidth, 30);

    clippingIndicatorView.setBounds (500, 20, clippingIndicatorViewWidth, 100);

    // s_freq.setBounds (20, 80, 100, 100);
    // s_freq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 30);
    // s_freqLabel.setBounds (20, 60, 100, 30);

    // constexpr int distX = 200;
    // b_chorusOn.setBounds (20 + distX, 90, 60, 40);
    // b_chorusOnLabel.setBounds (20 + distX, 60, 100, 30);

    constexpr unsigned int numCol = 8;
    constexpr unsigned int initX = 20;
    constexpr unsigned int initY = 60;
    constexpr unsigned int paramWidth = 100;
    constexpr unsigned int paramHeight = 100;
    constexpr unsigned int textEntryBoxWidth = 100;
    constexpr unsigned int textEntryBoxHeight = 30;

    for (int i = 0; i < paramLayout.size(); i++)
    {
        const auto& p = paramLayout[i];
        const auto& paramName = p.first;

        const unsigned int row = i / numCol;
        const unsigned int col = i % numCol;
        const unsigned int x /*of left top*/ = col * paramWidth + initX;
        const unsigned int y /*of left top*/ = row * paramHeight + initY;

        if (p.second == ParamType::SLIDER)
        {
            constexpr unsigned int knobMargin = 20;
            // s_freq.setBounds (20, 80, 100, 100);
            // s_freq.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 100, 30);
            // s_freqLabel.setBounds (20, 60, 100, 30);
            sliders[paramName]->setBounds (x, y + 20, paramWidth - knobMargin, paramHeight - knobMargin);
            sliders[paramName]->setTextBoxStyle (juce::Slider::TextBoxBelow, false, textEntryBoxWidth, textEntryBoxHeight);
            sliderLabels[paramName]->setBounds (x, y, textEntryBoxWidth, textEntryBoxHeight);
        }
        else /*(p.second == ParamType::BUTTON)*/
        {
            constexpr unsigned int buttonMargin = 40;
            // b_chorusOn.setBounds (20 + distX, 90, 60, 40);
            // b_chorusOnLabel.setBounds (20 + distX, 60, 100, 30);
            buttons[paramName]->setBounds (x, y + 30, paramWidth - buttonMargin, paramHeight - buttonMargin);
            buttonLabels[paramName]->setBounds (x, y, textEntryBoxWidth, textEntryBoxHeight);
        }
    }
}

void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (onsen::colors::backgroundColorDark));
}