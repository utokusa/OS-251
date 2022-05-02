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
          voicesToNote (getMaxNumVoices(), INIT_NOTE_NUMBER),
          pitchBendValue (INIT_PITCHBEND_VALUE),
          numVoices (INIT_NUMBER_OF_VOICES),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master())
    {
        assert (voices.size() == voicesToNote.size());
    }

    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo->setCurrentPlaybackSampleRate (sampleRate);
        for (int i = 0; i < getMaxNumVoices(); i++)
            voices[i]->setCurrentPlaybackSampleRate (sampleRate);
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
        for (int i = 0; i < getMaxNumVoices(); i++)
            voices[i]->renderNextBlock (outputAudio, startSample, numSamples);
        hpf.render (outputAudio, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (outputAudio, startSample, numSamples);
        masterVolume.render (outputAudio, startSample, numSamples);
    }

    void noteOn (int noteNumber, int intVelocity)
    {
        assert (0 <= noteNumber && noteNumber <= 127);
        assert (0 <= intVelocity && intVelocity <= 127);
        if (intVelocity == 0)
        {
            noteOff (noteNumber);
            return;
        }
        flnum velocity = static_cast<flnum> (intVelocity) / 127.0;

        // If there is already the same note, reuse the voice
        for (int i = 0; i < numVoices; i++)
        {
            if (voicesToNote[i] == noteNumber)
            {
                voices[i]->stopNote (0.0, false);
                lfo->noteOff();
                voices[i]->startNote (noteNumber, velocity, pitchBendValue);
                lfo->noteOn();
                return;
            }
        }

        // Try to find a free voice. If found, use it
        for (int i = 0; i < numVoices; i++)
        {
            if (isVoiceAvailable (i))
            {
                voices[i]->startNote (noteNumber, velocity, pitchBendValue);
                voicesToNote[i] = noteNumber;
                lfo->noteOn();
                return;
            }
        }

        // The case where there is no free voice, use the voice with the largest ID.
        int voiceIdToUse = numVoices - 1;
        assert (0 <= voiceIdToUse && voiceIdToUse < getMaxNumVoices());
        voices[voiceIdToUse]->stopNote (0.0, false);
        lfo->noteOff();
        voices[voiceIdToUse]->startNote (noteNumber, velocity, pitchBendValue);
        voicesToNote[voiceIdToUse] = noteNumber;
        lfo->noteOn();
    }

    void noteOff (int noteNumber)
    {
        for (int i = 0; i < numVoices; i++)
        {
            if (noteNumber == voicesToNote[i])
            {
                voices[i]->stopNote (0.0, true);
                lfo->noteOff();
                voicesToNote[i] = INIT_NOTE_NUMBER;
                return;
            }
        }
    }

    void allNoteOff()
    {
        for (int i = 0; i < numVoices; i++)
        {
            voices[i]->stopNote (0.0, true);
            voicesToNote[i] = INIT_NOTE_NUMBER;
        }
        lfo->allNoteOff();
    }

    void updatePitchWheel (int val)
    {
        assert (0 <= val && val <= 16383); // Should be treated like a signed 14 bit integer
        pitchBendValue = val;
        for (int i = 0; i < numVoices; i++)
            voices[i]->pitchWheelMoved (pitchBendValue);
    }

    static constexpr int getMaxNumVoices()
    {
        return MAX_NUM_VOICES;
    }

    void setNumberOfVoices (int num)
    {
        assert (1 <= num && num <= getMaxNumVoices());
        int diff = num - numVoices;
        int prev = numVoices;
        numVoices = num;
        if (diff < 0)
        {
            for (int i = numVoices; i < prev; i++)
            {
                voices[i]->stopNote (0.0, true);
                voicesToNote[i] = INIT_NOTE_NUMBER;
            }
            lfo->noteOff();
        }
    }

private:
    SynthParams* const params;
    Lfo* lfo;
    std::vector<std::shared_ptr<ISynthVoice>>& voices;
    std::vector<int> voicesToNote;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    int pitchBendValue;
    int numVoices;
    static constexpr int MAX_NUM_VOICES = 24;
    static constexpr int INIT_PITCHBEND_VALUE = 8192; // no pitchbend
    static constexpr int INIT_NOTE_NUMBER = -1;
    static constexpr int INIT_NUMBER_OF_VOICES = 1;

    bool isVoiceAvailable (int voiceId)
    {
        assert (0 <= voiceId && voiceId < getMaxNumVoices());
        return voicesToNote[voiceId] == INIT_NOTE_NUMBER;
    }
};
} // namespace onsen
