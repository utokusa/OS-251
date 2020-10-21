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

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (paramIdList[i][j].isNotEmpty())
            {
                sliderList[i][j] = std::make_unique<ParamSlider> (parameters, paramIdList[i][j]);
                addAndMakeVisible (*sliderList[i][j]);
            }
            else
            {
                sliderList[i][j] = nullptr;
            }
        }
    }

    setSize (600, 350);
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
    int deltaX = 0;
    int deltaY = 0;

    for (int i = 0; i < numRows; i++)
    {
        for (int j = 0; j < numCols; j++)
        {
            if (sliderList[i][j])
            {
                if (!deltaX || !deltaY)
                {
                    // Initialize deltaX and deltaY if needed
                    deltaX = sliderList[i][j]->getWidth();
                    deltaY = sliderList[i][j]->getHeight();
                }
                sliderList[i][j]->setBounds (j * deltaX, i * deltaY, deltaX, deltaY);
            }
        }
    }
}
