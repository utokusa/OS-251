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
#include "params/HpfParams.h"
#include "params/LfoParams.h"
#include "params/MasterParams.h"
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
    HpfParams* hpf()
    {
        return &hpfParams;
    }
    MasterParams* master()
    {
        return &masterParams;
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        oscillatorParams.prepareToPlay (samplesPerBlockExpected, sampleRate);
    }

private:
    // plugin parameters
    EnvelopeParams envelopeParams;
    LfoParams lfoParams;
    FilterParams filterParams;
    OscillatorParams oscillatorParams;
    ChorusParams chorusParams;
    HpfParams hpfParams;
    MasterParams masterParams;
};
} // namespace onsen
