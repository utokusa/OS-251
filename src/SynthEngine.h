/*
  ==============================================================================

    OS-251's synthesiser engine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"
#include "Chorus.h"
#include "Lfo.h"
#include "MasterVolume.h"
#include "SynthVoice.h"

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
          chorus(),
          masterVolume(synthParams)
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

private:
    SynthParams* const synthParams;
    Lfo lfo;
    FancySynth synth;
    juce::MidiMessageCollector midiCollector;
};
} // namespace onsen
