/*
  ==============================================================================

   Filter Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include <atomic>

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
    void setFrequencyPtr (const std::atomic<flnum>* _frequency)
    {
        frequency = _frequency;
        frequencyVal = *frequency;
    }
    flnum getResonance() const override
    {
        return lowestResVal() * std::pow (resBaseNumber(), resonanceVal);
    }
    void setResonancePtr (const std::atomic<flnum>* _resonance)
    {
        resonance = _resonance;
        resonanceVal = *resonance;
    }
    flnum getFilterEnvelope() const override
    {
        return DspUtil::valMinusOneToOne (filterEnvelopeVal);
    }
    void setFilterEnvelopePtr (const std::atomic<flnum>* _filterEnvelope)
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

private:
    const std::atomic<flnum>* frequency {};
    const std::atomic<flnum>* resonance {};
    const std::atomic<flnum>* filterEnvelope {};

    flnum frequencyVal = 0.0;
    flnum resonanceVal = 0.0;
    flnum filterEnvelopeVal = 0.0;
};
} // namespace onsen
