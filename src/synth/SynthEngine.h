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
    OnsenSynth (SynthParams* const synthParams, Lfo* const _lfo)
        : params (synthParams),
          lfo (_lfo),
          voice (params, lfo),
          pitchBendValue (8192 /*no pitchbend*/),
          currentNoteNumber (INIT_NOTE_NUMBER),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master()) {}
    void setCurrentPlaybackSampleRate (double sampleRate)
    {
        lfo->setCurrentPlaybackSampleRate (sampleRate);
        voice.setCurrentPlaybackSampleRate (sampleRate);
        chorus.setCurrentPlaybackSampleRate (sampleRate);
        hpf.setCurrentPlaybackSampleRate (sampleRate);
    }
    void setSamplesPerBlock (int samplesPerBlock)
    {
        lfo->setSamplesPerBlock (samplesPerBlock);
    }

    void renderNextBlock (IAudioBuffer* outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        if (! inputMidi.getNumEvents())
        {
            renderHelper (outputAudio, startSample, numSamples);
            return;
        }
        int firstEventTime = inputMidi.getFirstEventTime();
        renderHelper (outputAudio, startSample, std::max (0, firstEventTime - startSample));
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
                    if (midiMsg.getNoteNumber() == currentNoteNumber)
                    {
                        voice.stopNote (0.0, true);
                        lfo->noteOff();
                        currentNoteNumber = INIT_NOTE_NUMBER;
                        std::cout << "Midi note off..." << std::endl;
                    }
                }
                else if (midiMsg.isNoteOn())
                {
                    if (currentNoteNumber != INIT_NOTE_NUMBER)
                    {
                        voice.stopNote (0.0, false);
                        lfo->noteOff();
                    }
                    int intVelocity = static_cast<int> (midiMsg.getVelocity());
                    flnum velocity = static_cast<flnum> (intVelocity) / 127.0;
                    currentNoteNumber = midiMsg.getNoteNumber();
                    voice.startNote (midiMsg.getNoteNumber(), velocity, pitchBendValue);
                    lfo->noteOn();
                    std::cout << "Note on... note number:" << midiMsg.getNoteNumber() << std::endl;
                    std::cout << "velocity:" << (int) (midiMsg.getVelocity()) << std::endl;
                }
                else if (midiMsg.isAllNotesOff())
                {
                    voice.stopNote (0.0, true);
                    lfo->allNoteOff();
                    std::cout << "Midi all notes off..." << std::endl;
                }
                else if (midiMsg.isPitchWheel())
                {
                    pitchBendValue = midiMsg.getPitchWheelValue();
                    voice.pitchWheelMoved (pitchBendValue);
                }
                it++;
            }
            // render audio
            int numSamplesOfSection = it == inputMidi.end() ? startSample + numSamples - curSample : (*it).samplePosition - curSample;
            renderHelper (outputAudio, curSample, numSamplesOfSection);
        }
    }

private:
    SynthParams* const params;
    Lfo* const lfo;
    FancySynthVoice voice;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;
    int pitchBendValue;
    int currentNoteNumber;
    static constexpr int INIT_NOTE_NUMBER = -1;

    void renderHelper (IAudioBuffer* outputAudioBuffer, int startSample, int numSamples)
    {
        lfo->renderLfo (startSample, numSamples);
        lfo->renderLfoSync (startSample, numSamples);
        voice.renderNextBlock (outputAudioBuffer, startSample, numSamples);
        hpf.render (outputAudioBuffer, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (outputAudioBuffer, startSample, numSamples);
        masterVolume.render (outputAudioBuffer, startSample, numSamples);
    }
};

//==============================================================================
class SynthEngine
{
public:
    SynthEngine() = delete;
    SynthEngine (SynthParams* const _synthParams, IPositionInfo* const _positionInfo)
        : synthParams (_synthParams),
          positionInfo (_positionInfo),
          lfo (synthParams->lfo(), positionInfo),
          synth (synthParams, &lfo)
    {
        // for (auto i = 0; i < 4; ++i)
        //     synth.addVoice (new FancySynthVoice (synthParams, &lfo));

        // synth.addSound (new FancySynthSound());
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        synth.setSamplesPerBlock (samplesPerBlockExpected);
    }

    void releaseResources() {}

    void renderNextBlock (IAudioBuffer* outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        synth.renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
    }

    void changeNumberOfVoices (int num)
    {
        // int numVoices = synth.getNumVoices();
        // if (num == numVoices)
        //     return;
        // if (num > numVoices)
        //     addNumberOfVoices (num - numVoices);
        // else
        //     subNumberOfVoices (numVoices - num);

        // assert (num == synth.getNumVoices());
    }

private:
    SynthParams* const synthParams;
    IPositionInfo* positionInfo;
    Lfo lfo;
    OnsenSynth synth;

    void addNumberOfVoices (int num)
    {
        //  for (auto i = 0; i < num; ++i)
        //      synth.addVoice (new FancySynthVoice (synthParams, &lfo));
    }

    void subNumberOfVoices (int num)
    {
        // int numVoices = synth.getNumVoices();
        // assert (numVoices >= num);
        // for (auto i = 0; i < num; ++i)
        //     synth.removeVoice (numVoices - 1 - i);
    }
};
} // namespace onsen
