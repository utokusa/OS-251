/*
  ==============================================================================

   Juce AudioBuffer

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "../dsp/IAudioBuffer.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class JuceAudioBuffer : public IAudioBuffer
{
public:
    JuceAudioBuffer() = delete;
    JuceAudioBuffer (juce::AudioBuffer<flnum>* _audioBuffer) : audioBuffer (_audioBuffer) {}

    int getNumChannels() const noexcept override
    {
        return audioBuffer->getNumChannels();
    }

    int getNumSamples() const noexcept override
    {
        return audioBuffer->getNumSamples();
    }

    flnum* getWritePointer (int channel) noexcept override
    {
        return audioBuffer->getWritePointer (channel);
    }

    flnum getSample (int channel, int sampleIndex) const noexcept override
    {
        return audioBuffer->getSample (channel, sampleIndex);
    }

    void setSample (int destChannel, int destSample, flnum newValue) noexcept override
    {
        audioBuffer->setSample (destChannel, destSample, newValue);
    }

private:
    juce::AudioBuffer<flnum>* audioBuffer;
};
} // namespace onsen
