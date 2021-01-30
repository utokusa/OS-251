/*
  ==============================================================================

   Lfo

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"

namespace onsen
{
//==============================================================================
class Lfo
{
public:
    Lfo() = delete;
    Lfo (SynthParams* const synthParams)
        : p (synthParams->lfo()),
          sampleRate (DEFAULT_SAMPLE_RATE),
          numNoteOn (0),
          samplesPerBlock (DEFAULT_SAMPLES_PER_BLOCK),
          buf (samplesPerBlock),
          currentAngle (0.0),
          amp (0.0)
    {
    }

    void noteOn()
    {
        bool firstNote = (numNoteOn == 0);
        ++numNoteOn;
        if (firstNote)
        {
            constexpr flnum ampNoteStart = MAX_LEVEL * 0.01;
            amp = ampNoteStart;
        }
    }

    void noteOff()
    {
        numNoteOn = (--numNoteOn >= 0) ? numNoteOn : 0;
    }

    void allNoteOff()
    {
        numNoteOn = 0;
    }

    flnum getLevel (int sample)
    {
        assert (sample < buf.size());
        return buf[sample];
    }

    void renderLfo (int startSample, int numSamples)
    {
        int idx = startSample;
        while (--numSamples >= 0)
        {
            assert (idx < buf.size());
            buf[idx++] = lfoWave (currentAngle) * amp;
            flnum angleDelta = getAngleDelta();
            currentAngle += angleDelta;
            if (currentAngle > pi * 2.0)
            {
                currentAngle -= pi * 2.0;
            }
            updateAmp();
        }
    }

    flnum getPitchAmount()
    {
        return p->getPitch();
    }

    flnum getFilterFreqAmount()
    {
        return p->getFilterFreq();
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = static_cast<flnum> (_sampleRate);
    }

    void setSamplesPerBlock (int _samplesPerBlock)
    {
        samplesPerBlock = _samplesPerBlock;
        buf.resize (samplesPerBlock);
    }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    const LfoParams* const p;

    flnum sampleRate;
    int numNoteOn;

    // ---
    int samplesPerBlock;
    std::vector<flnum> buf;
    flnum currentAngle;
    flnum amp;
    // ---

    static flnum lfoWave (flnum angle)
    {
        return MAX_LEVEL * std::sin (angle);
    }

    flnum getAngleDelta()
    {
        flnum rate = p->getRate();
        return 2.0 * pi * rate / sampleRate;
    }

    void updateAmp()
    {
        constexpr flnum valFinishDelay = MAX_LEVEL * 0.99;
        // Value of adjust (getDelay()) is around 0.99
        amp = amp * MAX_LEVEL / adjust (p->getDelay());
        if (amp >= valFinishDelay)
        {
            amp = MAX_LEVEL;
        }
    }

    // Adjust parameter value like attack, decay or release according to the
    // sampling rate
    flnum adjust (const flnum val) const
    {
        // If no need to adjust
        if (std::abs (sampleRate - DEFAULT_SAMPLE_RATE) <= EPSILON)
        {
            return val;
        }
        flnum amount = std::pow (val, DEFAULT_SAMPLE_RATE / sampleRate - 1);
        return val * amount;
    }
};
}
