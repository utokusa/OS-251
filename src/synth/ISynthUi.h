/*
  ==============================================================================

    OS-251's synthesiser ui interface

  ==============================================================================
*/

#pragma once

namespace onsen
{
// This class is can be used by GUI classes.
// Normally they just use SynthUi class as a concrete class,
// but they can also use MockSynthUi class for development purpose.
class ISynthUi
{
public:
    virtual ~ISynthUi() = default;
    virtual bool isClipping() = 0;
};

} // namespace onsen
