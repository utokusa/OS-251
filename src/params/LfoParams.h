/*
  ==============================================================================

   LFO Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class ILfoParams
{
public:
    virtual flnum getRate() const = 0;
    virtual flnum getRateSync() const = 0;
    virtual flnum getPhase() const = 0;
    virtual flnum getDelay() const = 0;
    virtual bool getSyncOn() const = 0;
    virtual flnum getPitch() const = 0;
    virtual flnum getFilterFreq() const = 0;
    virtual flnum getShape() const = 0;
};

//==============================================================================
class LfoParams : public ILfoParams
{
public:
    // Returns LFO rate in [Hz].
    flnum getRate() const override
    {
        return lowestRateVal() * pow (rateBaseNumber(), rateVal);
    }
    void setRatePtr (const std::atomic<flnum>* _rate)
    {
        rate = _rate;
        rateVal = *rate;
    }
    // Returns syncd LFO rate in [quarter note].
    flnum getRateSync() const override
    {
        // The range of the return value is [1/48, 96/48]
        return static_cast<flnum> (
                   DspUtil::mapFlnumToInt (rateSyncVal, 0.0, 1.0, lowestRateSyncNumeratorVal(), highestRateSyncNumeratorVal(), true))
               / rateSyncDenominatorVal();
    }
    void setRateSyncPtr (const std::atomic<flnum>* _rateSync)
    {
        rateSync = _rateSync;
        rateSyncVal = *rateSync;
    }
    flnum getPhase() const override
    {
        constexpr flnum minVal = 0.0;
        constexpr flnum maxVal = 2 * pi;
        return minVal + (phaseVal) * (maxVal - minVal);
    }
    void setPhasePtr (const std::atomic<flnum>* _phase)
    {
        phase = _phase;
        phaseVal = *phase;
    }
    flnum getDelay() const override
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
    bool getSyncOn() const override
    {
        return syncOnVal > 0.5;
    }
    void setSyncOnPtr (const std::atomic<flnum>* _syncOn)
    {
        syncOn = _syncOn;
        syncOnVal = *syncOn;
    }
    flnum getPitch() const override
    {
        return ZeroOneToZeroOne::square (pitchVal);
    }
    void setPitchPtr (const std::atomic<flnum>* _pitch)
    {
        pitch = _pitch;
        pitchVal = *pitch;
    }
    flnum getFilterFreq() const override
    {
        return filterFreqVal;
    }
    void setFilterFreqPtr (const std::atomic<flnum>* _filterFreq)
    {
        filterFreq = _filterFreq;
        filterFreqVal = *filterFreq;
    }
    flnum getShape() const override
    {
        return shapeVal;
    }
    void setShapePtr (const std::atomic<flnum>* _shape)
    {
        shape = _shape;
        shapeVal = *shape;
    }
    void parameterChanged()
    {
        rateVal = *rate;
        rateSyncVal = *rateSync;
        phaseVal = *phase;
        delayVal = *delay;
        syncOnVal = *syncOn;
        pitchVal = *pitch;
        filterFreqVal = *filterFreq;
        shapeVal = *shape;
    }

    // ---
    // Parameter converting consts
    // Rate
    static constexpr flnum lowestRateVal()
    {
        return 0.1; // [Hz]
    }
    static constexpr flnum rateBaseNumber()
    {
        return 500.0;
    }

    // Synced rate
    static constexpr int lowestRateSyncNumeratorVal()
    {
        return 1; // [Hz]
    }
    static constexpr int highestRateSyncNumeratorVal()
    {
        return 96;
    }
    static constexpr int rateSyncDenominatorVal()
    {
        return 48;
    }

private:
    // LFO rate
    const std::atomic<flnum>* rate {};
    const std::atomic<flnum>* rateSync {};
    // LFO phase
    const std::atomic<flnum>* phase {};
    // LFO delay
    const std::atomic<flnum>* delay {};
    // Sync On
    const std::atomic<flnum>* syncOn {};
    // Amount of modulation
    const std::atomic<flnum>* pitch {};
    const std::atomic<flnum>* filterFreq {};
    const std::atomic<flnum>* shape {};

    flnum rateVal = 0.0;
    flnum rateSyncVal = 0.0;
    flnum phaseVal = 0.0;
    flnum delayVal = 0.0;
    flnum syncOnVal = 0.0;
    flnum pitchVal = 0.0;
    flnum filterFreqVal = 0.0;
    flnum shapeVal = 0.0;
};
} // namespace onsen
