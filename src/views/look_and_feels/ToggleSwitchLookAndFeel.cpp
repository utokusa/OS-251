/*
  ==============================================================================

   ToggleSwitchLookAndFeel

  ==============================================================================
*/

#include "ToggleSwitchLookAndFeel.h"

namespace onsen
{
//==============================================================================
void ToggleSwitchLookAndFeel::drawTickBox (Graphics& g, Component&, float x, float y, float width, float height, bool ticked, bool isEnabled, bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown)
{
    Rectangle<float> toggleSwitchArea (0, 0, toggleSwitchWidth, toggleSwitchHeight);
    if (ticked)
    {
        g.drawImage (ToggleSwitchOnImage, toggleSwitchArea, RectanglePlacement::Flags::xLeft);
    }
    else
    {
        g.drawImage (ToggleSwitchOffImage, toggleSwitchArea, RectanglePlacement::Flags::xLeft);
    }
}
} // namespace onsen