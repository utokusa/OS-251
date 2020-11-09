/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../../../Source/SynthEngine.h"

#include <JuceHeader.h>
#include <iostream>

//==============================================================================
double sampleRate = 44100.0;
int numSample = 512;
int numChannel = 2;

int main (int argc, char* argv[])
{
    std::cout << "Synth engine start." << std::endl;

    SynthEngine synthEngine;
    SynthParams& synthParams (SynthParams::getInstance());

    std::atomic<float> sinGain (0.5f);
    std::atomic<float> squareGain (0.5f);
    std::atomic<float> sawGain (0.5f);
    std::atomic<float> subSquareGain (0.5f);

    std::atomic<float> attack (0.5f);
    std::atomic<float> decay (0.5f);
    std::atomic<float> sustain (0.5f);
    std::atomic<float> release (0.5f);

    std::atomic<float> frequency (0.5f);
    std::atomic<float> resonance (0.5f);
    std::atomic<float> filterEnvelope (0.5f);

    // Oscillator parameters
    OscillatorParams* const oscillatorParams = synthParams.oscillator();
    oscillatorParams->setSinGainPtr (&sinGain);
    oscillatorParams->setSquareGainPtr (&squareGain);
    oscillatorParams->setSawGainPtr (&sawGain);
    oscillatorParams->setSubSquareGainPtr (&subSquareGain);

    // Envelop parameters
    EnvelopeParams* const envelopeParams = synthParams.envelope();
    envelopeParams->setAttackPtr (&attack);
    envelopeParams->setDecayPtr (&decay);
    envelopeParams->setSustainPtr (&sustain);
    envelopeParams->setReleasePtr (&release);

    // Filter parameters
    FilterParams* const filterParams = synthParams.filter();
    filterParams->setFrequencyPtr (&frequency);
    filterParams->setResonancePtr (&resonance);
    filterParams->setFilterEnvelopePtr (&filterEnvelope);

    synthEngine.prepareToPlay (0, sampleRate);

    std::cout << "Synth engine is initialized." << std::endl;

    std::cout << "Create input." << std::endl;

    juce::AudioBuffer<float> outputAudio (numChannel, numSample);

    // juce::MidiMessage
    // MidiMessage (int byte1, int byte2, int byte3, double timeStamp = 0) noexcept;
    juce::MidiMessage message;

    // juce::MidiBuffer
    // void addEvent (const MidiMessage& midiMessage, int sampleNumber);
    juce::MidiBuffer inputMidi;

    std::cout << "Start process." << std::endl;
    synthEngine.renderNextBlock(outputAudio, inputMidi, 0, numSample);

    std::cout << "Finish." << std::endl;

    return 0;
}
