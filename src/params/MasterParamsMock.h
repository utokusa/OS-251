/*
  ==============================================================================

   Mastter Mcok

  ==============================================================================
*/

#pragma once

#include "MasterParams.h"

namespace onsen
{
struct MasterParamsMock : public IMasterParams
{
    MasterParamsMock (
        bool envForAmpOn,
        flnum pitchBendWidth,
        flnum masterOctaveTune,
        flnum masterSemitoneTune,
        flnum masterFineTune,
        flnum portamento,
        flnum masterVolume) : envForAmpOn (envForAmpOn),
                              pitchBendWidth (pitchBendWidth),
                              masterOctaveTune (masterOctaveTune),
                              masterSemitoneTune (masterSemitoneTune),
                              masterFineTune (masterFineTune),
                              portamento (portamento),
                              masterVolume (masterVolume) {}

    bool getEnvForAmpOn() const override { return envForAmpOn; }
    flnum getPitchBendWidth() const override { return pitchBendWidth; }
    flnum getMasterOctaveTune() const override { return masterOctaveTune; }
    flnum getMasterSemitoneTune() const override { return masterSemitoneTune; }
    flnum getMasterFineTune() const override { return masterFineTune; }
    flnum getPortamento() const override { return portamento; }
    flnum getMasterVolume() const override { return masterVolume; }
    void setMasterVolume (flnum _masterVolume) { masterVolume = _masterVolume; }

    bool envForAmpOn;
    flnum pitchBendWidth;
    flnum masterOctaveTune;
    flnum masterSemitoneTune;
    flnum masterFineTune;
    flnum portamento;
    flnum masterVolume;
};

} // namespace onsen
