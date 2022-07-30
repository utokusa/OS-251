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
#include <cstdlib>
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
        : pitchBendValue (INIT_PITCHBEND_VALUE),
          numVoices (INIT_NUMBER_OF_VOICES),
          isUnison (false),
          isSustainPedalDown (false),
          isSostenutoPedalDown (false),
          params (synthParams),
          lfo (lfo),
          voices (voices),
          voicesToNote (getMaxNumVoices(), INIT_NOTE_NUMBER),
          isUnderSostenutoPedal (getMaxNumVoices()),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master())
    {
        assert (voices.size() == voicesToNote.size());
        addPhaseOffsetToVoices();
    }

    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo->setCurrentPlaybackSampleRate (sampleRate);
        for (int i = 0; i < getMaxNumVoices(); i++)
            voices[i]->setCurrentPlaybackSampleRate (sampleRate);
        chorus.setCurrentPlaybackSampleRate (sampleRate);
        hpf.setCurrentPlaybackSampleRate (sampleRate);
        masterVolume.setCurrentPlaybackSampleRate (sampleRate);
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

        if (isUnison)
        {
            noteOnUnisonMode (noteNumber, velocity);
        }
        else
        {
            noteOnPolyMode (noteNumber, velocity);
        }
    }

    void noteOff (int noteNumber)
    {
        assert (0 <= noteNumber && noteNumber <= 127);
        if (isUnison)
        {
            noteOffUnisonMode (noteNumber);
        }
        else
        {
            noteOffPolyMode (noteNumber);
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

    void setPitchWheel (int val)
    {
        assert (0 <= val && val <= 16383); // Should be treated like a signed 14 bit integer
        pitchBendValue = val;
        for (int i = 0; i < numVoices; i++)
            voices[i]->setPitchWheel (pitchBendValue);
    }

    void setSustainPedalDown (bool val)
    {
        if (isSustainPedalDown == val)
        {
            return;
        }
        // The pedal's state has changed
        isSustainPedalDown = val;
        if (! isSustainPedalDown)
        {
            for (int i = 0; i < numVoices; i++)
            {
                if (voicesToNote[i] == WAITING_FOR_SUSTAIN_PEDAL_UP)
                {
                    voices[i]->stopNote (0.0, true);
                    voicesToNote[i] = INIT_NOTE_NUMBER;
                }
                lfo->noteOff();
            }
        }
    }

    void setSostenutoPedalDown (bool val)
    {
        if (isSostenutoPedalDown == val)
        {
            return;
        }
        // The pedal's state has changed
        isSostenutoPedalDown = val;
        if (isSostenutoPedalDown)
        {
            for (int i = 0; i < numVoices; i++)
            {
                if (voicesToNote[i] != INIT_NOTE_NUMBER)
                {
                    isUnderSostenutoPedal[i] = true;
                }
            }
        }
        else
        {
            for (int i = 0; i < numVoices; i++)
            {
                if (isUnderSostenutoPedal[i])
                {
                    isUnderSostenutoPedal[i] = false;
                    if (voicesToNote[i] != INIT_NOTE_NUMBER && voicesToNote[i] != WAITING_FOR_SUSTAIN_PEDAL_UP)
                    {
                        voices[i]->stopNote (0.0, true);
                        lfo->noteOff();
                        voicesToNote[i] = INIT_NOTE_NUMBER;
                    }
                }
            }
        }
    }

    static constexpr int getMaxNumVoices()
    {
        return OscillatorConfig::MAX_NUM_VOICES;
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

    void setIsUnison (bool val)
    {
        isUnison = val;
        allNoteOff();
        setDetune (val);
    }

    //==============================================================================
    // UI output

    bool isClipping()
    {
        return masterVolume.isClipping();
    }

private:
    int pitchBendValue;
    int numVoices;
    bool isUnison;
    bool isSustainPedalDown;
    bool isSostenutoPedalDown;
    SynthParams* const params;
    Lfo* lfo;
    std::vector<std::shared_ptr<ISynthVoice>>& voices;
    std::vector<int> voicesToNote;
    std::vector<bool> isUnderSostenutoPedal;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    static constexpr int INIT_PITCHBEND_VALUE = 8192; // no pitchbend
    //  --- for voicesToNote --->
    static constexpr int INIT_NOTE_NUMBER = -1;
    static constexpr int WAITING_FOR_SUSTAIN_PEDAL_UP = -2;
    // <--- for voicesToNote ---
    static constexpr int INIT_NUMBER_OF_VOICES = 1;

    bool isVoiceAvailable (int voiceId)
    {
        assert (0 <= voiceId && voiceId < getMaxNumVoices());
        return voicesToNote[voiceId] == INIT_NOTE_NUMBER;
    }

    void noteOnPolyMode (int noteNumber, flnum velocity)
    {
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

    void noteOffPolyMode (int noteNumber)
    {
        for (int i = 0; i < numVoices; i++)
        {
            if (noteNumber == voicesToNote[i])
            {
                if (isSustainPedalDown)
                {
                    voicesToNote[i] = WAITING_FOR_SUSTAIN_PEDAL_UP;
                }
                else if (isSostenutoPedalDown && isUnderSostenutoPedal[i])
                {
                    // Do nothing
                }
                else
                {
                    voices[i]->stopNote (0.0, true);
                    lfo->noteOff();
                    voicesToNote[i] = INIT_NOTE_NUMBER;
                }
                return;
            }
        }
    }

    void noteOnUnisonMode (int noteNumber, flnum velocity)
    {
        for (int i = 0; i < numVoices; i++)
        {
            if (! isVoiceAvailable (i))
            {
                voices[i]->stopNote (0.0, false);
                lfo->noteOff();
            }
            isUnderSostenutoPedal[i] = false; // regardress of isSostenutoPedalDown
            voices[i]->startNote (noteNumber, velocity, pitchBendValue);
            voicesToNote[i] = noteNumber;
            lfo->noteOn();
        }
    }

    void noteOffUnisonMode (int noteNumber)
    {
        if (noteNumber != voicesToNote[0]) // it checks for all available voices not only voice 0
        {
            return;
        }
        for (int i = 0; i < numVoices; i++)
        {
            if (isSustainPedalDown)
            {
                voicesToNote[i] = WAITING_FOR_SUSTAIN_PEDAL_UP;
            }
            else if (isSostenutoPedalDown && isUnderSostenutoPedal[i])
            {
                // Do nothing
            }
            else
            {
                voices[i]->stopNote (0.0, true);
                voicesToNote[i] = INIT_NOTE_NUMBER;
                lfo->noteOff();
            }
        }
    }

    void addPhaseOffsetToVoices()
    {
        for (int i = 0; i < getMaxNumVoices(); i++)
        {
            flnum phaseOffset = static_cast<flnum> (rand()) / static_cast<flnum> (RAND_MAX); // [0, 1]
            phaseOffset *= 2 * pi; // [0, 2 * pi]
            voices[i]->addPhaseOffset (phaseOffset);
        }
    }

    void setDetune (bool val)
    {
        constexpr flnum maxDetuneVal = 0.2;
        if (val)
        {
            for (int i = 0; i < getMaxNumVoices(); i++)
            {
                flnum detune = static_cast<flnum> (i) / static_cast<flnum> (getMaxNumVoices()) * maxDetuneVal;
                if (i % 2)
                {
                    detune *= -1.0;
                }
                voices[i]->setDetune (detune);
            }
        }
        else
        {
            for (int i = 0; i < getMaxNumVoices(); i++)
            {
                voices[i]->setDetune (0.0);
            }
        }
    }
};
} // namespace onsen
