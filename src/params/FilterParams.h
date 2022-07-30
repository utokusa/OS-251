/*
  ==============================================================================

   Filter Parameters

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
class IFilterParams
{
public:
    virtual flnum getControlledFrequency (flnum controlVal) const = 0;
    virtual flnum getResonance() const = 0;
    virtual flnum getFilterEnvelope() const = 0;
};

//==============================================================================
class FilterParams : public IFilterParams
{
public:
    [[maybe_unused]] flnum getFrequency() const
    {
        return lowestFreqVal() * std::pow (freqBaseNumber(), frequencyVal);
    }

    flnum getControlledFrequency (flnum controlVal) const override
    {
        flnum newFrequency = std::clamp<flnum> (frequencyVal + controlVal, 0.0, 1.0);
        return lowestFreqVal() * std::pow (freqBaseNumber(), newFrequency);
    }

    void setFrequencyPtr (std::atomic<flnum>* _frequency)
    {
        frequency = _frequency;
        frequencyVal = *frequency;
    }

    flnum getResonance() const override
    {
        return lowestResVal() * std::pow (resBaseNumber(), resonanceVal);
    }

    void setResonancePtr (std::atomic<flnum>* _resonance)
    {
        resonance = _resonance;
        resonanceVal = *resonance;
    }

    flnum getFilterEnvelope() const override
    {
        return DspUtil::valMinusOneToOne (filterEnvelopeVal);
    }

    void setFilterEnvelopePtr (std::atomic<flnum>* _filterEnvelope)
    {
        filterEnvelope = _filterEnvelope;
        filterEnvelopeVal = *filterEnvelope;
    }

    void parameterChanged()
    {
        frequencyVal = *frequency;
        resonanceVal = *resonance;
        filterEnvelopeVal = *filterEnvelope;
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
    // Resonance
    static constexpr flnum lowestResVal()
    {
        return 0.2;
    }
    static constexpr flnum resBaseNumber()
    {
        return 100.0;
    }

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        constexpr int numDecimal = 4;
        return {
            { "frequency", "Frequency", 1.0, &frequency, [] (float value) { return ParamUtil::valueToFreqString (value, lowestFreqVal(), freqBaseNumber()); } },
            { "resonance", "Resonance", 0.35 /* converted to 1.0024*/, &resonance, [numDecimal] (float value) { return ParamUtil::valueToResString (value, lowestResVal(), resBaseNumber(), numDecimal); } },
            { "filterEnv", "Env -> Filter", 0.5, &filterEnvelope, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } }
        };
    }

private:
    std::atomic<flnum>* frequency {};
    std::atomic<flnum>* resonance {};
    std::atomic<flnum>* filterEnvelope {};

    flnum frequencyVal = 0.0;
    flnum resonanceVal = 0.0;
    flnum filterEnvelopeVal = 0.0;
};
} // namespace onsen
