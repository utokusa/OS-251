/*
  ==============================================================================

    OS-251's synthesiser engine

  ==============================================================================
*/

#pragma once

#include "../dsp/Chorus.h"
#include "../dsp/DspCommon.h"
#include "../dsp/Hpf.h"
#include "../dsp/IAudioBuffer.h"
#include "../dsp/IPositionInfo.h"
#include "../dsp/Lfo.h"
#include "../dsp/MasterVolume.h"
#include "SynthParams.h"
#include "SynthVoice.h"

namespace onsen
{
class SynthEngine
{
public:
    SynthEngine (SynthParams* const synthParams, IPositionInfo* const positionInfo, Lfo* lfo, ISynthVoice* voice)
        : params (synthParams),
          lfo (lfo),
          voice (voice),
          pitchBendValue (INIT_PITCHBEND_VALUE),
          currentNoteNumber (INIT_NOTE_NUMBER),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master()) {}
    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo->setCurrentPlaybackSampleRate (sampleRate);
        voice->setCurrentPlaybackSampleRate (sampleRate);
        chorus.setCurrentPlaybackSampleRate (sampleRate);
        hpf.setCurrentPlaybackSampleRate (sampleRate);
    }
    void setSamplesPerBlock (int samplesPerBlock)
    {
        lfo->setSamplesPerBlock (samplesPerBlock);
    }

    void renderNextBlock (IAudioBuffer* outputAudio, int startSample, int numSamples)
    {
        lfo->renderLfo (startSample, numSamples);
        lfo->renderLfoSync (startSample, numSamples);
        voice->renderNextBlock (outputAudio, startSample, numSamples);
        hpf.render (outputAudio, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (outputAudio, startSample, numSamples);
        masterVolume.render (outputAudio, startSample, numSamples);
    }

    void noteOn (int noteNumber, int intVelocity)
    {
        if (currentNoteNumber != INIT_NOTE_NUMBER)
        {
            voice->stopNote (0.0, false);
            lfo->noteOff();
        }
        flnum velocity = static_cast<flnum> (intVelocity) / 127.0;
        currentNoteNumber = noteNumber;
        voice->startNote (noteNumber, velocity, pitchBendValue);
        lfo->noteOn();
    }

    void noteOff (int noteNumber)
    {
        if (noteNumber == currentNoteNumber)
        {
            voice->stopNote (0.0, true);
            lfo->noteOff();
            currentNoteNumber = INIT_NOTE_NUMBER;
        }
    }

    void allNoteOff()
    {
        voice->stopNote (0.0, true);
        lfo->allNoteOff();
    }

    void updatePitchWheel (int val)
    {
        pitchBendValue = val;
        voice->pitchWheelMoved (pitchBendValue);
    }

private:
    SynthParams* const params;
    Lfo* lfo;
    ISynthVoice* voice;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    int pitchBendValue;
    int currentNoteNumber;
    static constexpr int INIT_PITCHBEND_VALUE = 8192; // no pitchbend
    static constexpr int INIT_NOTE_NUMBER = -1;
};
} // namespace onsen
