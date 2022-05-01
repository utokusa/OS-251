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
#include <memory>
#include <vector>

namespace onsen
{
class SynthEngine
{
public:
    SynthEngine (
        SynthParams* const synthParams,
        IPositionInfo* const positionInfo,
        Lfo* lfo,
        std::vector<std::shared_ptr<ISynthVoice>>& voices)
        : params (synthParams),
          lfo (lfo),
          voices (voices),
          pitchBendValue (INIT_PITCHBEND_VALUE),
          currentNoteNumber (INIT_NOTE_NUMBER),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master()) {}
    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo->setCurrentPlaybackSampleRate (sampleRate);
        voices[0]->setCurrentPlaybackSampleRate (sampleRate);
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
        voices[0]->renderNextBlock (outputAudio, startSample, numSamples);
        hpf.render (outputAudio, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (outputAudio, startSample, numSamples);
        masterVolume.render (outputAudio, startSample, numSamples);
    }

    void noteOn (int noteNumber, int intVelocity)
    {
        if (currentNoteNumber != INIT_NOTE_NUMBER)
        {
            voices[0]->stopNote (0.0, false);
            lfo->noteOff();
        }
        flnum velocity = static_cast<flnum> (intVelocity) / 127.0;
        currentNoteNumber = noteNumber;
        voices[0]->startNote (noteNumber, velocity, pitchBendValue);
        lfo->noteOn();
    }

    void noteOff (int noteNumber)
    {
        if (noteNumber == currentNoteNumber)
        {
            voices[0]->stopNote (0.0, true);
            lfo->noteOff();
            currentNoteNumber = INIT_NOTE_NUMBER;
        }
    }

    void allNoteOff()
    {
        voices[0]->stopNote (0.0, true);
        lfo->allNoteOff();
    }

    void updatePitchWheel (int val)
    {
        pitchBendValue = val;
        voices[0]->pitchWheelMoved (pitchBendValue);
    }

    static constexpr int getMaxNumVoices()
    {
        return MAX_NUM_VOICES;
    }

private:
    SynthParams* const params;
    Lfo* lfo;
    std::vector<std::shared_ptr<ISynthVoice>>& voices;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    int pitchBendValue;
    int currentNoteNumber;
    static constexpr int MAX_NUM_VOICES = 24;
    static constexpr int INIT_PITCHBEND_VALUE = 8192; // no pitchbend
    static constexpr int INIT_NOTE_NUMBER = -1;
};
} // namespace onsen
