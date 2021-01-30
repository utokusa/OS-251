/*
  ==============================================================================

   Filter

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"
#include "Envelope.h"
#include "Lfo.h"

namespace onsen
{
//==============================================================================
class Filter
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
    Filter() = delete;
    Filter (SynthParams* const synthParams, Envelope* const _env, Lfo* const _lfo)
        : p (synthParams->filter()),
          env (_env),
          lfo (_lfo),
          sampleRate (DEFAULT_SAMPLE_RATE),
          fb()
    {
    }

    flnum process (flnum sampleVal, int sampleIdx)
    {
        // Set biquad parameter coefficients
        // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
        const flnum freq = p->getControlledFrequency (env->getLevel() * p->getFilterEnvelope()
                                                    + lfo->getFilterFreqAmount() * lfo->getLevel (sampleIdx));
        const flnum omega0 = 2.0 * pi * freq / sampleRate;
        const flnum sinw0 = std::sin (omega0);
        const flnum cosw0 = std::cos (omega0);
        // sp.getResonance() stands for "Q".
        const flnum alpha = sinw0 / 2.0 / p->getResonance();
        const flnum a0 = 1.0 + alpha;
        const flnum a1 = -2.0 * cosw0;
        const flnum a2 = 1.0 - alpha;
        const flnum b0 = (1 - cosw0) / 2.0;
        const flnum b1 = 1 - cosw0;
        const flnum b2 = (1 - cosw0) / 2.0;

        const flnum out0 = b0 / a0 * sampleVal + b1 / a0 * fb.in1 + b2 / a0 * fb.in2
            - a1 / a0 * fb.out1 - a2 / a0 * fb.out2;
        fb.in2 = fb.in1;
        fb.in1 = sampleVal;

        fb.out2 = fb.out1;
        fb.out1 = out0;

        return out0;
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = static_cast<flnum> (_sampleRate);
    }

private:
    const FilterParams* const p;
    Envelope* const env;
    Lfo* const lfo;
    flnum sampleRate;
    // The length of this vector equals to max number of the channels;
    FilterBuffer fb;
};
}
