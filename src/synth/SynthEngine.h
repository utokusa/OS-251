/*
  ==============================================================================

    OS-251's synthesiser engine

  ==============================================================================
*/

#pragma once

#include "../dsp/Chorus.h"
#include "../dsp/DspCommon.h"
#include "../dsp/Hpf.h"
#include "../dsp/IPositionInfo.h"
#include "../dsp/JuceAudioBuffer.h"
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

    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
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
                    voice.stopNote (0.0, true);
                    lfo->noteOff();
                    std::cout << "Midi note off..." << std::endl;
                }
                else if (midiMsg.isNoteOn())
                {
                    int intVelocity = static_cast<int> (midiMsg.getVelocity());
                    flnum velocity = static_cast<flnum> (intVelocity) / 127.0;
                    voice.startNote (midiMsg.getNoteNumber(), velocity, nullptr, pitchBendValue);
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

    void renderHelper (juce::AudioBuffer<float>& outputAudio, int startSample, int numSamples)
    {
        JuceAudioBuffer outputAudioBuffer (&outputAudio);
        lfo->renderLfo (startSample, numSamples);
        lfo->renderLfoSync (startSample, numSamples);
        voice.renderNextBlock (outputAudio, startSample, numSamples);
        hpf.render (&outputAudioBuffer, startSample, numSamples);
        if (params->chorus()->getChorusOn())
            chorus.render (&outputAudioBuffer, startSample, numSamples);
        masterVolume.render (&outputAudioBuffer, startSample, numSamples);
    }
};

//==============================================================================
class FancySynth : public juce::Synthesiser
{
    using flnum = float;

public:
    FancySynth() = delete;
    FancySynth (SynthParams* const synthParams, Lfo* const _lfo)
        : params (synthParams),
          lfo (_lfo),
          hpf (params->hpf(), 2),
          chorus(),
          masterVolume (synthParams->master())
    {
    }

    void setCurrentPlaybackSampleRate (double sampleRate) override;
    void setSamplesPerBlock (int samplesPerBlock);
    void noteOn (int midiChannel,
                 int midiNoteNumber,
                 float velocity) override;
    void noteOff (int midiChannel,
                  int midiNoteNumber,
                  float velocity,
                  bool allowTailOff) override;
    void allNotesOff (int midiChannel,
                      bool allowTailOff) override;

private:
    SynthParams* const params;
    Lfo* const lfo;
    Hpf hpf;
    Chorus chorus;
    MasterVolume masterVolume;

    void renderVoices (juce::AudioBuffer<flnum>& outputAudio,
                       int startSample,
                       int numSamples) override;
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
        midiCollector.reset (sampleRate);
    }

    void releaseResources() {}

    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
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
    juce::MidiMessageCollector midiCollector;

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
