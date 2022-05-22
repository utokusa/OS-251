/*
  ==============================================================================

   Master volume

  ==============================================================================
*/

#pragma once

#include "../synth/SynthParams.h"
#include "DspCommon.h"
#include "IAudioBuffer.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class MasterVolume
{
public:
    MasterVolume (IMasterParams* const masterParams)
        : p (masterParams), sampleRate (DEFAULT_SAMPLE_RATE), remainingClipIndicateTimeSec (0.0), _isClipping (false) {}
    void render (IAudioBuffer* outputAudio, int startSample, int numSamples)
    {
        int idx = startSample;
        while (--numSamples >= 0)
        {
            const flnum gain = p->getMasterVolume();
            bool thisSampleClipping = false;
            for (auto i = outputAudio->getNumChannels(); --i >= 0;)
            {
                flnum inputVal = outputAudio->getSample (i, idx);
                flnum outputVal = inputVal * gain * gainAdjustment;
                // Clip the output audio
                thisSampleClipping |= std::abs (outputVal) > clippingValue;
                outputVal = std::clamp (outputVal, -clippingValue, clippingValue);
                outputAudio->setSample (i, idx, outputVal);
            }
            updateClippingIndicator (thisSampleClipping);
            idx++;
        }
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        assert (_sampleRate > 0.0);
        sampleRate = _sampleRate;
    }

    // Clipping indicator UI
    bool isClipping()
    {
        return _isClipping;
    }

private:
    static constexpr flnum gainAdjustment = 0.2;
    static constexpr flnum clippingValue = 2.0;
    const IMasterParams* const p;
    double sampleRate;
    static constexpr double maxClipIndicateTimeSec = 0.1;
    double remainingClipIndicateTimeSec;
    std::atomic<bool> _isClipping;

    void updateClippingIndicator (bool clippingSample)
    {
        if (clippingSample)
        {
            remainingClipIndicateTimeSec = maxClipIndicateTimeSec;
        }
        else
        {
            remainingClipIndicateTimeSec = std::max (0.0, remainingClipIndicateTimeSec - 1.0 / sampleRate);
        }
        _isClipping = remainingClipIndicateTimeSec > 0.0;
    }
};
} // namespace onsen
