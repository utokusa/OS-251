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
                outputAudio->setSample (i, idx, inputVal * gain);
            }
            idx++;
        }
    }

private:
    const IMasterParams* const p;
};
} // namespace onsen
