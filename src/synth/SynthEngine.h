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
#include <JuceHeader.h>

namespace onsen
{
class OnsenSynth
{
public:
    OnsenSynth (SynthParams* const synthParams, IPositionInfo* const positionInfo)
        : params (synthParams),
          lfo (params->lfo(), positionInfo),
          voice (params, &lfo),
          pitchBendValue (INIT_PITCHBEND_VALUE),
          currentNoteNumber (INIT_NOTE_NUMBER),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master()) {}
    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo.setCurrentPlaybackSampleRate (sampleRate);
        voice.setCurrentPlaybackSampleRate (sampleRate);
        chorus.setCurrentPlaybackSampleRate (sampleRate);
        hpf.setCurrentPlaybackSampleRate (sampleRate);
    }
    void setSamplesPerBlock (int samplesPerBlock)
    {
        lfo.setSamplesPerBlock (samplesPerBlock);
    }

    void renderNextBlock (IAudioBuffer* outputAudio, int startSample, int numSamples)
    {
        lfo.renderLfo (startSample, numSamples);
        lfo.renderLfoSync (startSample, numSamples);
        voice.renderNextBlock (outputAudio, startSample, numSamples);
        hpf.render (outputAudio, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (outputAudio, startSample, numSamples);
        masterVolume.render (outputAudio, startSample, numSamples);
    }

    void noteOn (int noteNumber, int intVelocity)
    {
        if (currentNoteNumber != INIT_NOTE_NUMBER)
        {
            voice.stopNote (0.0, false);
            lfo.noteOff();
        }
        flnum velocity = static_cast<flnum> (intVelocity) / 127.0;
        currentNoteNumber = noteNumber;
        voice.startNote (noteNumber, velocity, pitchBendValue);
        lfo.noteOn();
        std::cout << "Note on... note number:" << noteNumber << std::endl;
        std::cout << "velocity:" << intVelocity << std::endl;
    }

    void noteOff (int noteNumber)
    {
        if (noteNumber == currentNoteNumber)
        {
            voice.stopNote (0.0, true);
            lfo.noteOff();
            currentNoteNumber = INIT_NOTE_NUMBER;
            std::cout << "Midi note off..." << std::endl;
        }
    }

    void allNoteOff()
    {
        voice.stopNote (0.0, true);
        lfo.allNoteOff();
        std::cout << "Midi all notes off..." << std::endl;
    }

    void updatePitchWheel (int val)
    {
        pitchBendValue = val;
        voice.pitchWheelMoved (pitchBendValue);
    }

private:
    SynthParams* const params;
    Lfo lfo;
    FancySynthVoice voice;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    int pitchBendValue;
    int currentNoteNumber;
    static constexpr int INIT_PITCHBEND_VALUE = 8192; // no pitchbend
    static constexpr int INIT_NOTE_NUMBER = -1;
};

//==============================================================================
// TODO: Rename it to SynthEngineAdopter
class SynthEngine
{
public:
    SynthEngine() = delete;
    SynthEngine (SynthParams* const synthParams, IPositionInfo* const positionInfo)
        : synth (synthParams, positionInfo)
    {
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        synth.setSamplesPerBlock (samplesPerBlockExpected);
    }

    void releaseResources() {}

    void renderNextBlock (IAudioBuffer* outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        if (! inputMidi.getNumEvents())
        {
            synth.renderNextBlock (outputAudio, startSample, numSamples);
            return;
        }
        int firstEventTime = inputMidi.getFirstEventTime();
        synth.renderNextBlock (outputAudio, startSample, std::max (0, firstEventTime - startSample));
        auto it = inputMidi.begin();
        while (it != inputMidi.end())
        {
            auto metadata = *it;
            int curSample = metadata.samplePosition;
            while (it != inputMidi.end() && curSample == (*it).samplePosition)
            {
                // consume event
                auto midiMsg = (*it).getMessage();
                int midiStatusByte = (int) (*(midiMsg.getRawData()));
                if (midiMsg.isNoteOff())
                {
                    synth.noteOff (midiMsg.getNoteNumber());
                }
                else if (midiMsg.isNoteOn())
                {
                    synth.noteOn (midiMsg.getNoteNumber(), static_cast<int> (midiMsg.getVelocity()));
                }
                else if (midiMsg.isAllNotesOff())
                {
                    synth.allNoteOff();
                }
                else if (midiMsg.isPitchWheel())
                {
                    synth.updatePitchWheel (midiMsg.getPitchWheelValue());
                }
                it++;
            }
            // render audio
            int numSamplesOfSection = it == inputMidi.end() ? startSample + numSamples - curSample : (*it).samplePosition - curSample;
            synth.renderNextBlock (outputAudio, curSample, numSamplesOfSection);
        }
    }

    void changeNumberOfVoices (int num)
    {
    }

private:
    OnsenSynth synth;

    void addNumberOfVoices (int num)
    {
    }

    void subNumberOfVoices (int num)
    {
    }
};
} // namespace onsen
