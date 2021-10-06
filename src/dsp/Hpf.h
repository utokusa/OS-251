/*
  ==============================================================================

   High pass filter (HPF)

  ==============================================================================
*/

#pragma once

#include "../synth/SynthParams.h"
#include "DspCommon.h"
#include "Envelope.h"
#include "IAudioBuffer.h"
#include "Lfo.h"

namespace onsen
{
//==============================================================================
class Hpf
{
    using flnum = float;
    struct FilterBuffer
    {
    public:
        FilterBuffer() : in1 (0.0), in2 (0.0), out1 (0.0), out2 (0.0) {}
        ~FilterBuffer() = default;
        ;
        flnum in1, in2;
        flnum out1, out2;
    };

public:
    Hpf() = delete;
    Hpf (IHpfParams* const hpfParams, int _numChannels)
        : p (hpfParams),
          sampleRate (DEFAULT_SAMPLE_RATE),
          numChannels (_numChannels),
          filterBuffers (numChannels),
          smoothedFreq (0.0, 0.999)
    {
        smoothedFreq.reset (p->getFrequency());
    }

    void render (IAudioBuffer* outputAudio, int startSample, int numSamples)
    {
        // Set biquad parameter coefficients
        // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html

        flnum targetFreq = p->getFrequency();
        smoothedFreq.set (targetFreq);

        int numInputChannels = outputAudio->getNumChannels();
        int bufferSize = outputAudio->getNumSamples();
        for (int i = startSample; i < bufferSize && i < startSample + numSamples; i++)
        {
            smoothedFreq.update();
        }

        flnum omega0 = 2.0f * 3.14159265f * smoothedFreq.get() / sampleRate;
        flnum sinw0 = std::sin (omega0);
        flnum cosw0 = std::cos (omega0);
        constexpr flnum resonance = 1.0;
        flnum alpha = sinw0 / 2.0 / resonance;
        flnum a0 = 1.0 + alpha;
        flnum a1 = -2.0 * cosw0;
        flnum a2 = 1.0 - alpha;
        flnum b0 = (1 + cosw0) / 2.0;
        flnum b1 = -1 - cosw0;
        flnum b2 = (1 + cosw0) / 2.0;

        // Calculate output

        for (int channel = 0; channel < std::min (numChannels, numInputChannels); channel++)
        {
            FilterBuffer& fb = filterBuffers[channel];
            flnum* bufferPtr = outputAudio->getWritePointer (channel);
            for (int i = startSample; i < bufferSize && i < startSample + numSamples; i++)
            {
                flnum out0 = b0 / a0 * bufferPtr[i] + b1 / a0 * fb.in1 + b2 / a0 * fb.in2
                             - a1 / a0 * fb.out1 - a2 / a0 * fb.out2;
                fb.in2 = fb.in1;
                fb.in1 = bufferPtr[i];

                fb.out2 = fb.out1;
                fb.out1 = out0;

                bufferPtr[i] = out0;
            }
        }
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = static_cast<flnum> (_sampleRate);
        smoothedFreq.prepareToPlay (_sampleRate);
    }

private:
    const IHpfParams* const p;
    flnum sampleRate;
    int numChannels;
    // The length of this vector equals to max number of the channels;
    std::vector<FilterBuffer> filterBuffers;
    SmoothFlnum smoothedFreq;
};
} // namespace onsen
