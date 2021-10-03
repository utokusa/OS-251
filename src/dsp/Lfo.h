/*
  ==============================================================================

   Lfo

  ==============================================================================
*/

#pragma once

#include "../synth/SynthParams.h"
#include "DspCommon.h"
#include "IPositionInfo.h"
#include <vector>

namespace onsen
{
//==============================================================================
class Lfo
{
public:
    Lfo() = delete;
    Lfo (ILfoParams* const lfoParams, const IPositionInfo* _positionInfo)
        : p (lfoParams),
          positionInfo (_positionInfo),
          sampleRate (DEFAULT_SAMPLE_RATE),
          numNoteOn (0),
          samplesPerBlock (DEFAULT_SAMPLES_PER_BLOCK),
          buf (samplesPerBlock),
          bufSync (samplesPerBlock),
          currentAngle (0.0),
          currentAngleSync (0.0),
          amp (0.0),
          ampSync (0.0),
          isPlaying (false),
          basePosistionInQuarterNote (0.0),
          baseAngle (0.0)
    {
    }

    void noteOn()
    {
        const bool firstNote = (numNoteOn == 0);
        ++numNoteOn;
        if (firstNote)
        {
            constexpr flnum ampNoteStart = MAX_LEVEL * 0.01;
            amp = ampNoteStart;
            ampSync = ampNoteStart;
            const flnum phase = p->getPhase();
            currentAngle = 0.0 + phase;
            currentAngleSync = 0.0 + phase;
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

    flnum getLevel (int sample) const
    {
        if (p->getSyncOn())
        {
            assert (sample < bufSync.size());
            return bufSync[sample];
        }
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
            const flnum angleDelta = getAngleDelta();
            currentAngle += angleDelta;
            if (currentAngle > pi * 2.0)
            {
                currentAngle -= pi * 2.0;
            }
            updateAmp();
        }
    }

    void renderLfoSync (int startSample, int numSamples)
    {
        int idx = startSample;

        if (! positionInfo)
        {
            // We don't have host with tempo functionality
            return;
        }

        const flnum bpm = positionInfo->getBpm();

        if (! isPlaying && positionInfo->isPlaying())
        {
            // When DAW starts to play
            isPlaying = true;
            basePosistionInQuarterNote = positionInfo->getPpqPosition();
            baseAngle = currentAngle;
        }

        if (isPlaying && ! positionInfo->isPlaying())
        {
            // When DAW stop playing
            isPlaying = false;
        }

        const flnum beatsPerSec = positionInfo->getBpm() / 60.0; // [quarter note / sec]
        const flnum quarterNotesFromBaseToStartIdx = positionInfo->getPpqPosition() - basePosistionInQuarterNote; // [quarter note]
        while (--numSamples >= 0)
        {
            // LFO angle from the time DAW starts to play.
            flnum angleFromBase = 0.0; // initialize
            if (isPlaying)
            {
                assert (idx < bufSync.size());
                const flnum timeFromBufStartToIdx = (idx - startSample) / sampleRate; // [sec]
                const flnum quarterNotesFromBaseToIdx = quarterNotesFromBaseToStartIdx
                                                        + beatsPerSec * timeFromBufStartToIdx; // [quarter note]
                const flnum barFromBaseToIdx = quarterNotesFromBaseToIdx / 4;
                angleFromBase = barFromBaseToIdx / p->getRateSync() * 2 * pi - baseAngle;
            }

            const flnum angleAccumulated = currentAngleSync + angleDelta (bpm);

            // In general we want to use angleFromBase because it's
            // more accurate than angleAccumulated.
            // However, we cannot use use angleFromBase in some cases.
            // e.g. DAW stops playing. Or DAW uses audio play with loop.
            // In such cases, angleFromBase might be much different from true value,
            // so we use angleAccumulated instead.
            if (isPlaying && std::abs (angleFromBase - angleAccumulated) < 0.1)
            {
                currentAngleSync = angleFromBase;
            }
            else
            {
                currentAngleSync = angleAccumulated;
            }

            bufSync[idx++] = lfoWave (currentAngleSync) * ampSync;

            if (currentAngleSync > pi * 2.0)
            {
                currentAngleSync -= pi * 2.0;
            }

            updateAmpSync();
        }
    }

    flnum getPitchAmount() const
    {
        return p->getPitch();
    }

    flnum getFilterFreqAmount() const
    {
        return p->getFilterFreq();
    }

    flnum getShapeAmount() const
    {
        return p->getShape();
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = static_cast<flnum> (_sampleRate);
    }

    void setSamplesPerBlock (int _samplesPerBlock)
    {
        samplesPerBlock = _samplesPerBlock;
        buf.resize (samplesPerBlock);
        bufSync.resize (samplesPerBlock);
    }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    const ILfoParams* const p;
    const IPositionInfo* positionInfo;

    flnum sampleRate;
    int numNoteOn;

    // ---
    int samplesPerBlock;
    std::vector<flnum> buf;
    std::vector<flnum> bufSync;
    flnum currentAngle;
    flnum currentAngleSync;
    flnum amp;
    flnum ampSync;

    // ---
    // For tempo on
    bool isPlaying;
    // DAW postion when play starts
    flnum basePosistionInQuarterNote;
    // DAW angle when play starts
    flnum baseAngle;

    // ---

    static flnum lfoWave (flnum angle)
    {
        return MAX_LEVEL * std::sin (angle);
    }

    flnum getAngleDelta() const
    {
        const flnum rate = p->getRate();
        return 2.0 * pi * rate / sampleRate;
    }

    flnum angleDelta (flnum bpm) const
    {
        const flnum barInSec = 1.0 / bpm /*[min / quarter note]*/ * 60.0 * 4; // [sec]
        const flnum deltaTime = 1.0 / sampleRate; // [sec]
        const flnum period = p->getRateSync() * barInSec; // [bar] * [sec / bar] = [sec]
        const flnum deltaAngle = 2.0 * pi * deltaTime / period; // [rad]
        return deltaAngle;
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

    void updateAmpSync()
    {
        constexpr flnum valFinishDelay = MAX_LEVEL * 0.99;
        // Value of adjust (getDelay()) is around 0.99
        ampSync = ampSync * MAX_LEVEL / adjust (p->getDelay());
        if (ampSync >= valFinishDelay)
        {
            ampSync = MAX_LEVEL;
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
        const flnum amount = std::pow (val, DEFAULT_SAMPLE_RATE / sampleRate - 1);
        return val * amount;
    }
};
} // namespace onsen
