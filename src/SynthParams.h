/*
  ==============================================================================

   OS-251 synthesizer's parameters

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "params/ChorusParams.h"
#include "params/EnvelopeParams.h"
#include "params/FilterParams.h"
#include "params/LfoParams.h"
#include "params/OscillatorParams.h"

namespace onsen
{
//==============================================================================
class SynthParams
{
public:
    SynthParams() = default;
    EnvelopeParams* envelope()
    {
        return &envelopeParams;
    }
    LfoParams* lfo()
    {
        return &lfoParams;
    }
    FilterParams* filter()
    {
        return &filterParams;
    }
    OscillatorParams* oscillator()
    {
        return &oscillatorParams;
    }
    ChorusParams* chorus()
    {
        return &chorusParams;
    }

private:
    // plugin parameters
    EnvelopeParams envelopeParams;
    LfoParams lfoParams;
    FilterParams filterParams;
    OscillatorParams oscillatorParams;
    ChorusParams chorusParams;
};
}
