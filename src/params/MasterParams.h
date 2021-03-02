/*
  ==============================================================================

   Master Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"

namespace onsen
{
//==============================================================================
class MasterParams
{
public:
    static constexpr int dynamicRange = 48;
    static const int maxOctaveTuneVal = 3; // unit is [octave]
    static const int maxSemitoneTuneVal = 12; // unit is [semitone] or [st]

    //==============================================================================
    flnum getMasterOctaveTune() const
    {
        return DspUtil::mapFlnumToInt (masterOctaveTuneVal, 0.0, 1.0, -maxOctaveTuneVal, maxOctaveTuneVal);
    }
    void setMasterOctaveTunePtr (const std::atomic<flnum>* _masterOctaveTune)
    {
        masterOctaveTune = _masterOctaveTune;
        masterOctaveTuneVal = *masterOctaveTune;
    }
    flnum getMasterSemitoneTune() const
    {
        return DspUtil::mapFlnumToInt (masterSemitoneTuneVal, 0.0, 1.0, -maxSemitoneTuneVal, maxSemitoneTuneVal);
    }
    void setMasterSemitoneTunePtr (const std::atomic<flnum>* _masterSemitoneTune)
    {
        masterSemitoneTune = _masterSemitoneTune;
        masterSemitoneTuneVal = *masterSemitoneTune;
    }
    flnum getMasterFineTune() const
    {
        return DspUtil::valMinusOneToOne (masterFineTuneVal);
    }
    void setMasterFineTunePtr (const std::atomic<flnum>* _masterFineTune)
    {
        masterFineTune = _masterFineTune;
        masterFineTuneVal = *masterFineTune;
    }
    flnum getMasterVolume() const
    {
        auto decibelGain = DspUtil::paramValToDecibel (masterVolumeVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setMasterVolumePtr (const std::atomic<flnum>* _masterVolume)
    {
        masterVolume = _masterVolume;
        masterVolumeVal = *masterVolume;
    }
    void parameterChanged()
    {
        masterOctaveTuneVal = *masterOctaveTune;
        masterSemitoneTuneVal = *masterSemitoneTune;
        masterFineTuneVal = *masterFineTune;
        masterVolumeVal = *masterVolume;
    }
    flnum getFreqRatio()
    {
        return std::pow (
            2.0,
            getMasterOctaveTune() + (getMasterSemitoneTune() + getMasterFineTune()) / 12.0);
    }

private:
    const std::atomic<flnum>* masterOctaveTune {};
    const std::atomic<flnum>* masterSemitoneTune {};
    const std::atomic<flnum>* masterFineTune {};
    const std::atomic<flnum>* masterVolume {};

    flnum masterOctaveTuneVal = 0.5;
    flnum masterSemitoneTuneVal = 0.5;
    flnum masterFineTuneVal = 0.5;
    flnum masterVolumeVal = 0.5;
};
} // namespace onsen
