/*
  ==============================================================================

   OS-251 synthesizer's parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "../params/ChorusParams.h"
#include "../params/EnvelopeParams.h"
#include "../params/FilterParams.h"
#include "../params/HpfParams.h"
#include "../params/LfoParams.h"
#include "../params/MasterParams.h"
#include "../params/OscillatorParams.h"

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
    }

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        std::vector<ParamMetaInfo> ret;

        auto envelopePML = envelopeParams.getParamMetaList();
        ret.insert (ret.end(), envelopePML.begin(), envelopePML.end());

        auto lfoPML = lfoParams.getParamMetaList();
        ret.insert (ret.end(), lfoPML.begin(), lfoPML.end());

        auto filterPML = filterParams.getParamMetaList();
        ret.insert (ret.end(), filterPML.begin(), filterPML.end());

        auto oscillatorPML = oscillatorParams.getParamMetaList();
        ret.insert (ret.end(), oscillatorPML.begin(), oscillatorPML.end());

        auto chorusPML = chorusParams.getParamMetaList();
        ret.insert (ret.end(), chorusPML.begin(), chorusPML.end());

        auto hpfPML = hpfParams.getParamMetaList();
        ret.insert (ret.end(), hpfPML.begin(), hpfPML.end());

        auto masterPML = masterParams.getParamMetaList();
        ret.insert (ret.end(), masterPML.begin(), masterPML.end());

        return ret;
    }

    void parameterChanged()
    {
        envelopeParams.parameterChanged();
        lfoParams.parameterChanged();
        filterParams.parameterChanged();
        oscillatorParams.parameterChanged();
        chorusParams.parameterChanged();
        hpfParams.parameterChanged();
        masterParams.parameterChanged();
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
