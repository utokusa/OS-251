/*
  ==============================================================================

   Master volume

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "IAudioBuffer.h"
#include "SynthParams.h"

namespace onsen
{
//==============================================================================
class MasterVolume
{
public:
    MasterVolume (IMasterParams* const masterParams)
        : p (masterParams) {}
    void render (IAudioBuffer* outputAudio, int startSample, int numSamples)
    {
        int idx = startSample;
        while (--numSamples >= 0)
        {
            const flnum gain = p->getMasterVolume();
            for (auto i = outputAudio->getNumChannels(); --i >= 0;)
            {
                flnum inputVal = outputAudio->getSample (i, idx);
                flnum outputVal = std::clamp (inputVal * gain * gainAdjustment, -clippingValue, clippingValue);
                outputAudio->setSample (i, idx, outputVal);
            }
            idx++;
        }
    }

private:
    static constexpr flnum gainAdjustment = 0.2;
    static constexpr flnum clippingValue = 3.0;
    const IMasterParams* const p;
};
} // namespace onsen
