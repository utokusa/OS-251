/*
  ==============================================================================

   Clipping Indicator View

  ==============================================================================
*/

#include "ClippingIndicatorView.h"

namespace onsen
{
//==============================================================================
ClippingIndicatorView::ClippingIndicatorView (ISynthUi* synthUi) : synthUi (synthUi)
{
    startTimerHz (30);
}

// Lights the LED if the limiter is working.
void ClippingIndicatorView::paint (juce::Graphics& g)
{
    g.setColour (juce::Colour (colors::textColor));

    g.setFont (12.0f);
    constexpr float adjustMarginTop = 1.0f;
    g.drawText (
        juce::String ("Clipping"),
        margin * 2.0f + largerCircleDiameter,
        margin + adjustMarginTop,
        textWidth,
        textHeight,
        juce::Justification::topLeft);
    bool isClipping = synthUi->isClipping();
    if (isClipping)
    {
        // LED is on.
        g.setColour (juce::Colour (clippingColorId));
    }
    else
    {
        // LED is off.
        g.setColour (juce::Colour (nonClippingColorId));
    }
    g.drawEllipse (margin + diffRadius, margin + diffRadius, smallerCircleDiameter, smallerCircleDiameter, circleThickness);
}
//==============================================================================
} // namespace onsen
