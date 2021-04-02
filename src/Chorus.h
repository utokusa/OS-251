/*
  ==============================================================================

   Chorus

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class Chorus
{
    struct ChorusLfo
    {
    public:
        flnum val() const
        {
            return sinWave(currentAngle);
        }

        void update()
        {
            currentAngle += angleDelta();
            if (currentAngle > 2.0 * pi)
                currentAngle -= 2.0 * pi;
        }

    private:
        flnum angleDelta() const
        {
            return 2.0 * pi * freq / sampleRate;
        }

        static flnum sinWave (flnum angle)
        {
            return std::sin (angle);
        }

    public:
        flnum currentAngle;
        flnum freq;
        const flnum& sampleRate;
    };

public:
    Chorus()
        : sampleRate (DEFAULT_SAMPLE_RATE),
          delayTime_msec (15.0),
          feedback (0.3),
          maxDelayTime_msec (20.0),
          writePointer (0),
          lfo ({ 0.0, 0.5, sampleRate }),
          depth (0.1),
          dryLevel (1.0),
          wetLevel (1.0)
    {
        prepare();
    };

    void render (juce::AudioBuffer<flnum>& outputAudio, int startSample, int numSamples);
    void setCurrentPlaybackSampleRate (double _sampleRate);

private:
    flnum sampleRate;
    flnum delayTime_msec;
    flnum feedback;
    flnum maxDelayTime_msec;
    std::vector<flnum> buf;
    int writePointer;
    ChorusLfo lfo;
    flnum depth;
    flnum dryLevel;
    flnum wetLevel;

    //==============================================================================
    void prepare();

    inline int delay_sample()
    {
        return static_cast<int> (delayTime_msec * (1.0 + depth * lfo.val()) / 1000.0 * sampleRate);
    }

    inline int readIdx()
    {
        const int idx = writePointer - delay_sample();
        return idx >= 0 ? idx : idx + static_cast<int> (buf.size());
    }
};
} // namespace onsen
