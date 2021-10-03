/*
  ==============================================================================

   Master Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class IMasterParams
{
public:
    virtual bool getEnvForAmpOn() const = 0;
    virtual flnum getPitchBendWidth() const = 0;
    virtual flnum getMasterOctaveTune() const = 0;
    virtual flnum getMasterSemitoneTune() const = 0;
    virtual flnum getMasterFineTune() const = 0;
    virtual flnum getPortamento() const = 0;
    virtual flnum getMasterVolume() const = 0;
};
//==============================================================================
class MasterParams : public IMasterParams
{
public:
    static constexpr int maxPitchBendWidth = 24; // unit is [semitone] or [st]
    static constexpr int dynamicRange = 48;
    static constexpr int maxOctaveTuneVal = 3; // unit is [octave]
    static constexpr int maxSemitoneTuneVal = 12; // unit is [semitone] or [st]
    static constexpr int maxNumVoices = 10;

    //==============================================================================
    bool getEnvForAmpOn() const override
    {
        return envForAmpOnVal;
    }
    void setEnvForAmpOnPtr (const std::atomic<flnum>* _envForAmpOn)
    {
        envForAmpOn = _envForAmpOn;
        envForAmpOnVal = *_envForAmpOn;
    }
    flnum getPitchBendWidth() const override
    {
        return DspUtil::mapFlnumToInt (pitchBendWidthVal, 0.0, 1.0, 0, maxPitchBendWidth);
    }
    void setPitchBendWidthPtr (const std::atomic<flnum>* _piatchBendWidth)
    {
        pitchBendWidth = _piatchBendWidth;
        pitchBendWidthVal = *pitchBendWidth;
    }
    flnum getMasterOctaveTune() const override
    {
        return DspUtil::mapFlnumToInt (masterOctaveTuneVal, 0.0, 1.0, -maxOctaveTuneVal, maxOctaveTuneVal);
    }
    void setMasterOctaveTunePtr (const std::atomic<flnum>* _masterOctaveTune)
    {
        masterOctaveTune = _masterOctaveTune;
        masterOctaveTuneVal = *masterOctaveTune;
    }
    flnum getMasterSemitoneTune() const override
    {
        return DspUtil::mapFlnumToInt (masterSemitoneTuneVal, 0.0, 1.0, -maxSemitoneTuneVal, maxSemitoneTuneVal);
    }
    void setMasterSemitoneTunePtr (const std::atomic<flnum>* _masterSemitoneTune)
    {
        masterSemitoneTune = _masterSemitoneTune;
        masterSemitoneTuneVal = *masterSemitoneTune;
    }
    flnum getMasterFineTune() const override
    {
        return DspUtil::valMinusOneToOne (masterFineTuneVal);
    }
    void setMasterFineTunePtr (const std::atomic<flnum>* _masterFineTune)
    {
        masterFineTune = _masterFineTune;
        masterFineTuneVal = *masterFineTune;
    }
    flnum getPortamento() const override
    {
        // Whwn knob value is 0, portamento is OFF
        if (portamentoVal == 0.0)
        {
            return 0.0;
        }

        constexpr flnum minPortament = 0.999;
        return minPortament + portamentoVal * (1.0 - minPortament);
    }
    void setPortamentoPtr (const std::atomic<flnum>* _portamento)
    {
        portamento = _portamento;
        portamentoVal = *portamento;
    }
    flnum getMasterVolume() const override
    {
        return masterVolumeVal;
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
        portamentoVal = *portamento;
        masterVolumeVal = *masterVolume;
    }
    flnum getPitchBendWidthInFreqRatio() const
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
    const std::atomic<flnum>* portamento {};
    const std::atomic<flnum>* masterVolume {};

    flnum envForAmpOnVal = 1.0;
    flnum pitchBendWidthVal = 12; // Unit is [semitone]
    flnum masterOctaveTuneVal = 0.5;
    flnum masterSemitoneTuneVal = 0.5;
    flnum masterFineTuneVal = 0.5;
    flnum portamentoVal = 0.0;
    flnum masterVolumeVal = 0.5;
};
} // namespace onsen
