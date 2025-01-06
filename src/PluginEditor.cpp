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
      clippingIndicatorView (_synthUi),
      buttonParamLaf()
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

    for (const auto& p : paramLayout)
    {
        const auto& paramName = p.first;
        if (p.second == ParamType::SLIDER)
        {
            sliders.try_emplace (paramName, std::make_unique<juce::Slider> (paramName));
            sliderLabels.try_emplace (paramName, std::make_unique<juce::Label>());
            sliderAttachment.try_emplace (paramName, std::make_unique<SliderAttachment> (valueTreeState, paramName, *sliders[paramName]));

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

            buttons[paramName]->setColour (juce::TextButton::textColourOnId, juce::Colour (onsen::colors::primaryColor));
            buttons[paramName]->setColour (juce::TextButton::textColourOffId, juce::Colour (onsen::colors::textColor));
            buttons[paramName]->setColour (juce::TextButton::buttonOnColourId, juce::Colour (onsen::colors::backgroundColorDark));
            buttons[paramName]->setColour (juce::TextButton::buttonColourId, juce::Colour (onsen::colors::backgroundColorDark));
            buttons[paramName]->setButtonText (valueTreeState.getParameter (paramName)->name);
            buttonLabels[paramName]->setText (valueTreeState.getParameter (paramName)->name, juce::dontSendNotification);
            addAndMakeVisible (*buttons[paramName]);

            buttons[paramName]->setClickingTogglesState (true);
            buttons[paramName]->setLookAndFeel (&buttonParamLaf);
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
    presetManagerView.setBounds (150, 20, 600 - 20 - clippingIndicatorViewWidth, 30);

    clippingIndicatorView.setBounds (640, 20, clippingIndicatorViewWidth, 100);

    for (int i = 0; i < paramLayout.size(); i++)
    {
        const auto& p = paramLayout[i];
        const auto& paramName = p.first;

        const unsigned int row = i / numCol;
        const unsigned int col = i % numCol;
        const unsigned int x /*of left top*/ = col * paramWidth + 8 + initX;
        const unsigned int y /*of left top*/ = row * rowHeight + 6 + initY;

        if (p.second == ParamType::SLIDER)
        {
            constexpr unsigned int knobMargin = 20;
            constexpr unsigned int width = paramWidth - knobMargin;
            sliders[paramName]->setBounds (x, y, width, paramHeight - knobMargin);
            constexpr bool isReadOnly = true;
            sliders[paramName]->setTextBoxStyle (juce::Slider::TextBoxBelow, isReadOnly, width, textEntryBoxHeight);
            sliderLabels[paramName]->setBounds (x, y + sliders[paramName]->getHeight() - 14, width, textEntryBoxHeight);
            sliderLabels[paramName]->setJustificationType (juce::Justification::centred);
        }
        else /*(p.second == ParamType::BUTTON)*/
        {
            constexpr unsigned int buttonMargin = 20;
            buttons[paramName]->setBounds (x, y + 12, paramWidth - buttonMargin, paramHeight - buttonMargin);
            buttonLabels[paramName]->setBounds (x, y, textEntryBoxWidth, textEntryBoxHeight);
        }
    }
}

void Os251AudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (onsen::colors::backgroundColorDark));

    // Synth Name
    g.setColour (juce::Colour (onsen::colors::textColor));
    g.setFont (23.0f);
    constexpr float adjustMarginTop = 1.0f;
    g.drawText (
        juce::String ("OS-251"),
        20,
        20,
        100,
        100,
        juce::Justification::topLeft);

    for (int row = 0; row < paramLayout.size() / numCol + (paramLayout.size() % numCol ? 1 : 0); row++)
    {
        const unsigned int x /*of left top*/ = 0 + initX;
        const unsigned int y /*of left top*/ = row * rowHeight + initY;
        constexpr float heightMargin = 1.0f;
        const juce::Rectangle bound = { static_cast<float> (x), static_cast<float> (y), static_cast<float> (paramWidth * numCol), static_cast<float> (paramHeight) - heightMargin };
        constexpr float cornerSize = 8.0f;

        g.setColour (juce::Colour (onsen::colors::backgroundColor));
        g.fillRoundedRectangle (bound, cornerSize);
        g.setColour (juce::Colour (onsen::colors::textColor));
        g.drawRoundedRectangle (bound, cornerSize, 1.5f);
    }
}