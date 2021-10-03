/*
  ==============================================================================

   Mock AudioBuffer

  ==============================================================================
*/

#pragma once

#include "../../../src/dsp/DspCommon.h"
#include "../../../src/dsp/IAudioBuffer.h"
#include <vector>

namespace onsen
{
//==============================================================================
class AudioBufferMock : public IAudioBuffer
{
public:
    AudioBufferMock (
        size_t numChannels = DEFAULT_NUM_CHANNELS,
        size_t samplesPerBlock = DEFAULT_SAMPLES_PER_BLOCK)
    {
        audioBuffer.resize (numChannels);
        for (auto& buf : audioBuffer)
        {
            buf.resize (samplesPerBlock);
        }
    };

    int getNumChannels() const noexcept override
    {
        return audioBuffer.size();
    }

    int getNumSamples() const noexcept override
    {
        if (audioBuffer.size() == 0)
        {
            return 0;
        }
        return audioBuffer[0].size();
    }

    flnum* getWritePointer (int channel) noexcept override
    {
        assert (channel < getNumChannels() && getNumSamples() > 0);
        return &audioBuffer[channel][0];
    }

    flnum getSample (int channel, int sampleIndex) const noexcept override
    {
        assert (channel < getNumChannels() && sampleIndex < getNumSamples());
        return audioBuffer[channel][sampleIndex];
    }

    void setSample (int destChannel, int destSample, flnum newValue) noexcept override
    {
        assert (destChannel < getNumChannels() && destSample < getNumSamples());
        audioBuffer[destChannel][destSample] = newValue;
    }

private:
    static constexpr size_t DEFAULT_BUFFER_SIZE = 512;
    static constexpr size_t DEFAULT_NUM_CHANNELS = 2;
    std::vector<std::vector<flnum>> audioBuffer;
};
} // namespace onsen
