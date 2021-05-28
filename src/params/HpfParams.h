/*
  ==============================================================================

   HPF Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class HpfParams
{
public:
    flnum getFrequency() const
    {
        return lowestFreqVal() * pow (freqBaseNumber(), frequencyVal);
    }
    void setFrequencyPtr (const std::atomic<flnum>* _frequency)
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

private:
    const std::atomic<flnum>* frequency {};

    flnum frequencyVal = 0.0;
};
} // namespace onsen
