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
    flnum getMasterVolume() const
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
        masterVolumeVal = *masterVolume;
    }

private:
    const std::atomic<flnum>* masterVolume {};
    flnum masterVolumeVal = 0.5;
};
}
