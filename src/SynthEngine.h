/*
  ==============================================================================

    OS-251's synthesiser engine

  ==============================================================================
*/

#pragma once

#include "Chorus.h"
#include "DspCommon.h"
#include "Hpf.h"
#include "Lfo.h"
#include "MasterVolume.h"
#include "SynthParams.h"
#include "SynthVoice.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class FancySynth : public juce::Synthesiser
{
    using flnum = float;

public:
    FancySynth() = delete;
    FancySynth (SynthParams* const synthParams, Lfo* const _lfo)
        : params (synthParams),
          lfo (_lfo),
          hpf (params, 2),
          chorus(),
          masterVolume (synthParams)
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
    SynthEngine (SynthParams* const _synthParams)
        : synthParams (_synthParams),
          lfo (synthParams),
          synth (synthParams, &lfo)
    {
        for (auto i = 0; i < 4; ++i)
            synth.addVoice (new FancySynthVoice (synthParams, &lfo));

        synth.addSound (new FancySynthSound());
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
        int numVoices = synth.getNumVoices();
        if (num == numVoices)
            return;
        if (num > numVoices)
            addNumberOfVoices (num - numVoices);
        else
            subNumberOfVoices (numVoices - num);
        
        assert(num == synth.getNumVoices());
    }

private:
    SynthParams* const synthParams;
    Lfo lfo;
    FancySynth synth;
    juce::MidiMessageCollector midiCollector;

    void addNumberOfVoices (int num)
    {
        for (auto i = 0; i < num; ++i)
            synth.addVoice (new FancySynthVoice (synthParams, &lfo));
    }

    void subNumberOfVoices (int num)
    {
        int numVoices = synth.getNumVoices();
        assert (numVoices >= num);
        for (auto i = 0; i < num; ++i)
            synth.removeVoice (numVoices - 1 - i);
    }
};
} // namespace onsen
