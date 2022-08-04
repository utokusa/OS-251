/*
  ==============================================================================

   HPF Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "ParamCommon.h"
#include <atomic>
#include <vector>

namespace onsen
{
//==============================================================================
class IHpfParams
{
public:
    virtual flnum getFrequency() const = 0;
};

//==============================================================================
class HpfParams : public IHpfParams
{
public:
    flnum getFrequency() const override
    {
        return lowestFreqVal() * pow (freqBaseNumber(), frequencyVal);
    }

    void setFrequencyPtr (std::atomic<flnum>* _frequency)
    {
        frequency = _frequency;
        frequencyVal = *frequency;
    }

    void parameterChanged()
    {
        frequencyVal = *frequency;
    }

    // ---
    // Parameter converting consts
    // Frequency
    static constexpr flnum lowestFreqVal()
    {
        return 20.0; // [Hz]
    }
    static constexpr flnum freqBaseNumber()
    {
        return 1000.0;
    }

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        return {
            { "hpfFreq", "HPF Freq", 0.0, &frequency, [] (float value) { return ParamUtil::valueToFreqString (value, lowestFreqVal(), freqBaseNumber()); } },
        };
    }

private:
    std::atomic<flnum>* frequency {};

    flnum frequencyVal = 0.0;
};
} // namespace onsen
