/*
  ==============================================================================

   Master Parameters

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

    //==============================================================================
    bool getEnvForAmpOn() const override
    {
        return envForAmpOnVal;
    }

    void setEnvForAmpOnPtr (std::atomic<flnum>* _envForAmpOn)
    {
        envForAmpOn = _envForAmpOn;
        envForAmpOnVal = *_envForAmpOn;
    }

    flnum getPitchBendWidth() const override
    {
        return DspUtil::mapFlnumToInt (pitchBendWidthVal, 0.0, 1.0, 0, maxPitchBendWidth);
    }

    void setPitchBendWidthPtr (std::atomic<flnum>* _piatchBendWidth)
    {
        pitchBendWidth = _piatchBendWidth;
        pitchBendWidthVal = *pitchBendWidth;
    }

    flnum getMasterOctaveTune() const override
    {
        return DspUtil::mapFlnumToInt (masterOctaveTuneVal, 0.0, 1.0, -maxOctaveTuneVal, maxOctaveTuneVal);
    }

    void setMasterOctaveTunePtr (std::atomic<flnum>* _masterOctaveTune)
    {
        masterOctaveTune = _masterOctaveTune;
        masterOctaveTuneVal = *masterOctaveTune;
    }

    flnum getMasterSemitoneTune() const override
    {
        return DspUtil::mapFlnumToInt (masterSemitoneTuneVal, 0.0, 1.0, -maxSemitoneTuneVal, maxSemitoneTuneVal);
    }

    void setMasterSemitoneTunePtr (std::atomic<flnum>* _masterSemitoneTune)
    {
        masterSemitoneTune = _masterSemitoneTune;
        masterSemitoneTuneVal = *masterSemitoneTune;
    }

    flnum getMasterFineTune() const override
    {
        return DspUtil::valMinusOneToOne (masterFineTuneVal);
    }

    void setMasterFineTunePtr (std::atomic<flnum>* _masterFineTune)
    {
        masterFineTune = _masterFineTune;
        masterFineTuneVal = *masterFineTune;
    }

    flnum getPortamento() const override
    {
        // When knob value is 0, portamento is OFF
        if (portamentoVal == 0.0)
        {
            return 0.0;
        }

        constexpr flnum minPortamento = 0.999;
        return minPortamento + portamentoVal * (1.0 - minPortamento);
    }

    void setPortamentoPtr (std::atomic<flnum>* _portamento)
    {
        portamento = _portamento;
        portamentoVal = *portamento;
    }
    flnum getMasterVolume() const override
    {
        return masterVolumeVal;
    }

    void setMasterVolumePtr (std::atomic<flnum>* _masterVolume)
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

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        constexpr int numDecimal = 4;
        return {
            { "envForAmpOn", "Env -> Amp", 1.0, &envForAmpOn, ParamUtil::valueToOnOffString },
            { "pitchBendWidth", "Pitch Bend", 0.5, &pitchBendWidth, [] (float value) { return std::to_string (
                                                                                           DspUtil::mapFlnumToInt (value, 0.0, 1.0, 0, maxPitchBendWidth)); } },
            { "masterOctaveTune", "Octave", 0.5, &masterOctaveTune, [] (float value) { return std::to_string (
                                                                                           DspUtil::mapFlnumToInt (value, 0.0, 1.0, -maxOctaveTuneVal, maxOctaveTuneVal)); } },
            { "masterSemitoneTune", "Semi", 0.5, &masterSemitoneTune, [] (float value) { return std::to_string (
                                                                                                    DspUtil::mapFlnumToInt (value, 0.0, 1.0, -maxSemitoneTuneVal, maxSemitoneTuneVal))
                                                                                                + " st"; } },
            { "masterFineTune", "Fine Tune", 0.5, &masterFineTune, [numDecimal] (float value) { return ParamUtil::valueToMinusOneToOneString (value, numDecimal); } },
            { "portamento", "Portamento", 0.0, &portamento, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "masterVolume", "Master Vol", 0.5, &masterVolume, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } }
        };
    }

private:
    std::atomic<flnum>* envForAmpOn {};
    std::atomic<flnum>* pitchBendWidth {};
    std::atomic<flnum>* masterOctaveTune {};
    std::atomic<flnum>* masterSemitoneTune {};
    std::atomic<flnum>* masterFineTune {};
    std::atomic<flnum>* portamento {};
    std::atomic<flnum>* masterVolume {};

    flnum envForAmpOnVal = 1.0;
    flnum pitchBendWidthVal = 12; // Unit is [semitone]
    flnum masterOctaveTuneVal = 0.5;
    flnum masterSemitoneTuneVal = 0.5;
    flnum masterFineTuneVal = 0.5;
    flnum portamentoVal = 0.0;
    flnum masterVolumeVal = 0.5;
};
} // namespace onsen
