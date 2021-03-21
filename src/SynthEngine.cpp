/*
  ==============================================================================

   OS-251's synthesiser engine

  ==============================================================================
*/

#include "SynthEngine.h"

namespace onsen
{
//==============================================================================
void FancySynth::setCurrentPlaybackSampleRate (double sampleRate)
{
    lfo->setCurrentPlaybackSampleRate (sampleRate);
    chorus.setCurrentPlaybackSampleRate (sampleRate);
    juce::Synthesiser::setCurrentPlaybackSampleRate (sampleRate);
}

void FancySynth::setSamplesPerBlock (int samplesPerBlock)
{
    lfo->setSamplesPerBlock (samplesPerBlock);
}

void FancySynth::noteOn (int midiChannel,
                         int midiNoteNumber,
                         float velocity)
{
    lfo->noteOn();
    juce::Synthesiser::noteOn (midiChannel, midiNoteNumber, velocity);
}

void FancySynth::noteOff (int midiChannel,
                          int midiNoteNumber,
                          float velocity,
                          bool allowTailOff)
{
    lfo->noteOff();
    juce::Synthesiser::noteOff (midiChannel, midiNoteNumber, velocity, allowTailOff);
}

void FancySynth::allNotesOff (int midiChannel,
                              bool allowTailOff)
{
    lfo->allNoteOff();
    juce::Synthesiser::allNotesOff (midiChannel, allowTailOff);
}

//==============================================================================
void FancySynth::renderVoices (juce::AudioBuffer<flnum>& outputAudio,
                               int startSample,
                               int numSamples)
{
    lfo->renderLfo (startSample, numSamples);
    lfo->renderLfoSync (startSample, numSamples);
    juce::Synthesiser::renderVoices (outputAudio, startSample, numSamples);
    hpf.render (outputAudio, startSample, numSamples);
    if (params->chorus()->getChorusOn())
        chorus.render (outputAudio, startSample, numSamples);
    masterVolume.render (outputAudio, startSample, numSamples);
}

} // namespace onsen
