/*
  ==============================================================================

   Chorus

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "IAudioBuffer.h"
#include <vector>

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
            return sinWave (currentAngle);
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
          wetLevel (1.0),
          interpolateBufferAccess (true)
    {
        prepare();
    };

    void render (IAudioBuffer* outputAudio, int startSample, int numSamples);
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
    bool interpolateBufferAccess;

    //==============================================================================
    void prepare();

    inline int delaySample()
    {
        return static_cast<int> (delayTime_msec * (1.0 + depth * lfo.val()) / 1000.0 * sampleRate);
    }

    inline int readIdx()
    {
        const int idx = writePointer - delaySample();
        return idx >= 0 ? idx : idx + static_cast<int> (buf.size());
    }

    inline flnum getModDelayValueWithoutInterpolation()
    {
        return buf.at (readIdx());
    }

    inline flnum delayTimeInSec()
    {
        return delayTime_msec * (1.0 + depth * lfo.val()) / 1000.0;
    }

    inline int firstReadIdx (const flnum delayTimeInSample)
    {
        const int idx = writePointer - (static_cast<int> (delayTimeInSample) + 1);

        return idx >= 0 ? idx : idx + static_cast<int> (buf.size());
    }

    inline flnum firstReadSampleRatio (const flnum delayTimeInSample)
    {
        return delayTimeInSample - std::floor (delayTimeInSample);
    }

    inline flnum getBufValueWithInterpolation (int firstIdx, int secondIdx, flnum firstRatio)
    {
        // [Circuit Bending]
        // return buf.at(firstIdx) * firstIdx + buf.at(secondIdx) * (1.0 - firstIdx);
        return buf.at (firstIdx) * firstRatio + buf.at (secondIdx) * (1.0 - firstRatio);
    }

    inline flnum getModDelayValueWithInterpolation()
    {
        // Use linear interpolation
        // https://ccrma.stanford.edu/~jos/pasp/Delay_Line_Interpolation.html
        const flnum delayTimeInSample = delayTimeInSec() * sampleRate;
        const flnum firstIdx = firstReadIdx (delayTimeInSample);
        const flnum secondIdx = (firstIdx == static_cast<int> (buf.size()) - 1) ? 0 : firstIdx + 1;
        const flnum firstRatio = firstReadSampleRatio (delayTimeInSample);
        return getBufValueWithInterpolation (firstIdx, secondIdx, firstRatio);
    }

    inline flnum getModDelayValue()
    {
        if (interpolateBufferAccess)
        {
            return getModDelayValueWithInterpolation();
        }
        else
        {
            // It has zipper noise
            return getModDelayValueWithoutInterpolation();
        }
    }
};
} // namespace onsen
