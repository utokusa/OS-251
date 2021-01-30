/*
  ==============================================================================

   LFO Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"

namespace onsen
{
//==============================================================================
class LfoParams
{
public:
    // Returns LFO rate in [Hz].
    flnum getRate() const
    {
        return lowestRateVal() * pow (rateBaseNumber(), rateVal);
    }
    void setRatePtr (const std::atomic<flnum>* _rate)
    {
        rate = _rate;
        rateVal = *rate;
    }
    flnum getDelay() const
    {
        constexpr flnum minVal = 0.995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (delayVal) * (maxVal - minVal);
    }
    void setDelayPtr (const std::atomic<flnum>* _rate)
    {
        delay = _rate;
        delayVal = *delay;
    }
    flnum getPitch() const
    {
        return pitchVal;
    }
    void setPitchPtr (const std::atomic<flnum>* _pitch)
    {
        pitch = _pitch;
        pitchVal = *pitch;
    }
    flnum getFilterFreq() const
    {
        return filterFreqVal;
    }
    void setFilterFreqPtr (const std::atomic<flnum>* _filterFreq)
    {
        filterFreq = _filterFreq;
        filterFreqVal = *filterFreq;
    }
    void parameterChanged()
    {
        rateVal = *rate;
        delayVal = *delay;
        pitchVal = *pitch;
        filterFreqVal = *filterFreq;
    }

    // ---
    // Parameter converting consts
    // Frequency
    static constexpr flnum lowestRateVal()
    {
        return 0.1; // [Hz]
    }
    static constexpr flnum rateBaseNumber()
    {
        return 500.0;
    }

private:
    // LFO rate
    const std::atomic<flnum>* rate {};
    // LFO delay
    const std::atomic<flnum>* delay {};
    // Amount of modulation
    const std::atomic<flnum>* pitch {};
    const std::atomic<flnum>* filterFreq {};

    flnum rateVal = 0.0;
    flnum delayVal = 0.0;
    flnum pitchVal = 0.0;
    flnum filterFreqVal = 0.0;
};
}
