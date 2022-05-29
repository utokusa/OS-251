/*
  ==============================================================================

    OS-251's synthesiser UI

  ==============================================================================
*/

#pragma once
#include "ISynthUi.h"
#include "SynthEngine.h"

namespace onsen
{
class SynthUi : public ISynthUi
{
public:
    SynthUi (SynthEngine& synth) : synth (synth) {}
    ~SynthUi() override = default;
    bool isClipping() override
    {
        return synth.isClipping();
    };

private:
    SynthEngine& synth;
};

} // namespace onsen
