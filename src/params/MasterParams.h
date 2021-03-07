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
    static constexpr int maxPitchBendWidth = 24; // unit is [semitone] or [st]
    static constexpr int dynamicRange = 48;
    static const int maxOctaveTuneVal = 3; // unit is [octave]
    static const int maxSemitoneTuneVal = 12; // unit is [semitone] or [st]

    //==============================================================================
    bool getEnvForAmpOn() const
    {
        return envForAmpOnVal;
    }
    void setEnvForAmpOnPtr (const std::atomic<flnum>* _envForAmpOn)
    {
        envForAmpOn = _envForAmpOn;
        envForAmpOnVal = *_envForAmpOn;
    }
    flnum getPitchBendWidth() const
    {
        return DspUtil::mapFlnumToInt(pitchBendWidthVal, 0.0, 1.0, 0, maxPitchBendWidth);
    }
    void setPitchBendWidthPtr (const std::atomic<flnum>* _piatchBendWidth)
    {
        pitchBendWidth = _piatchBendWidth;
        pitchBendWidthVal = *pitchBendWidth;
    }
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
        envForAmpOnVal = *envForAmpOn;
        pitchBendWidthVal = *pitchBendWidth;
        masterOctaveTuneVal = *masterOctaveTune;
        masterSemitoneTuneVal = *masterSemitoneTune;
        masterFineTuneVal = *masterFineTune;
        masterVolumeVal = *masterVolume;
    }
    flnum getPitchBendWidthInFreqRatio () const
    {
        // pitchBendWidthVal is in [semitone].
        // return it in frequency ratio.
        return std::pow (2.0, getPitchBendWidth() / 12.0);
    }
    flnum getFreqRatio()
    {
        // Return frequency ratio of pitch tuning
        return std::pow (
            2.0,
            getMasterOctaveTune() + (getMasterSemitoneTune() + getMasterFineTune()) / 12.0);
    }

private:
    const std::atomic<flnum>* envForAmpOn {};
    const std::atomic<flnum>* pitchBendWidth {};
    const std::atomic<flnum>* masterOctaveTune {};
    const std::atomic<flnum>* masterSemitoneTune {};
    const std::atomic<flnum>* masterFineTune {};
    const std::atomic<flnum>* masterVolume {};

    flnum envForAmpOnVal = 1.0;
    flnum pitchBendWidthVal = 12; // Unit is [semitone]
    flnum masterOctaveTuneVal = 0.5;
    flnum masterSemitoneTuneVal = 0.5;
    flnum masterFineTuneVal = 0.5;
    flnum masterVolumeVal = 0.5;
};
} // namespace onsen
