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

    //==============================================================================
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
        masterVolumeVal = *masterVolume;
    }

private:
    const std::atomic<flnum>* masterVolume {};
    flnum masterVolumeVal = 0.5;
};
} // namespace onsen
