/*
  ==============================================================================

   Master volume

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"

namespace onsen
{
//==============================================================================
class MasterVolume
{
public:
    MasterVolume (SynthParams* const synthParams)
    : p (synthParams->master()) {}
    void render (juce::AudioBuffer<flnum>& outputAudio, int startSample, int numSamples)
    {
        int idx = startSample;
        while (--numSamples >= 0)
        {
            const flnum gain = p->getMasterVolume();
            for (auto i = outputAudio.getNumChannels(); --i >= 0;)
            {
                flnum inputVal = outputAudio.getSample (i, idx);
                outputAudio.setSample(i, idx, inputVal * gain);
            }
            idx++;
        }
    }
private:
    const MasterParams* const p;
};
}
