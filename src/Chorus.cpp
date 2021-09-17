/*
  ==============================================================================

   Chorus

  ==============================================================================
*/

#include "Chorus.h"

namespace onsen
{
//==============================================================================
void Chorus::render (IAudioBuffer* outputAudio, int startSample, int numSamples)
{
    int idx = startSample;
    while (--numSamples >= 0)
    {
        // Convert input to mono
        flnum monoInputVal = 0.0;
        for (auto i = outputAudio->getNumChannels(); --i >= 0;)
        {
            monoInputVal += outputAudio->getSample (i, idx);
        }
        monoInputVal /= outputAudio->getNumChannels();

        const flnum delayVal = getModDelayValue();
        buf.at (writePointer) = monoInputVal + delayVal * feedback;
        flnum outputVal = monoInputVal * dryLevel + delayVal * wetLevel;
        for (auto i = outputAudio->getNumChannels(); --i >= 0;)
        {
            outputAudio->setSample (i, idx, outputVal);
        }

        // Update LFO's state
        lfo.update();
        idx++;
        writePointer = (writePointer + 1) % buf.size();
    }
}

void Chorus::setCurrentPlaybackSampleRate (double _sampleRate)
{
    sampleRate = _sampleRate;
    prepare();
}

void Chorus::prepare()
{
    const auto bufSize = static_cast<int> (sampleRate * maxDelayTime_msec / 1000.0);
    assert (bufSize > 0);
    buf.resize (bufSize, 0.0);
    writePointer = 0;
}
} // namespace onsen
