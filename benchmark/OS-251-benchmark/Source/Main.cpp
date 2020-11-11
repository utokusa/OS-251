/*
  ==============================================================================

    Benchmark for synthesizer engine

  ==============================================================================
*/

#include "../../../Source/SynthEngine.h"

#include <JuceHeader.h>
#include <algorithm>
#include <benchmark/benchmark.h>

//==============================================================================
// Constants

constexpr double SAMPLE_RATE = 44100.0;
constexpr double MAX_TIME_SEC = 2.0;
constexpr int NUM_SAMPLE = static_cast<int> (SAMPLE_RATE) * static_cast<int> (MAX_TIME_SEC);
constexpr int NUM_CHANNEL = 2;
// First byte of a midi message
constexpr int NOTE_ON = 0x90;

// Notes
constexpr int C1 = 0x3c;
[[maybe_unused]] constexpr int CS1 = 0x3d;
constexpr int D1 = 0x3e;
[[maybe_unused]] constexpr int DS1 = 0x3f;
constexpr int E1 = 0x40;
constexpr int F1 = 0x41;
[[maybe_unused]] constexpr int FS1 = 0x42;
constexpr int G1 = 0x43;
[[maybe_unused]] constexpr int GS1 = 0x44;
constexpr int A1 = 0x45;
[[maybe_unused]] constexpr int AS1 = 0x46;
constexpr int B1 = 0x47;

// Velocity
constexpr int VEL_100 = 0x64; // 100 in decimal
constexpr int VEL_OFF = 0x00;

//==============================================================================

class SynthEngineFixture : public benchmark::Fixture
{
public:
    void SetUp (::benchmark::State& state) override
    {
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

        synthEngine.prepareToPlay (0, SAMPLE_RATE);

        for (const auto& note : notes)
        {
            inputMidiBuffer.addEvent (note, timeSecToSample (note.getTimeStamp(), SAMPLE_RATE, NUM_SAMPLE));
        }
    }

    void TearDown (::benchmark::State& state) override
    {
    }

    void render()
    {
        synthEngine.renderNextBlock (outputAudio, inputMidiBuffer, 0, NUM_SAMPLE);
    }

    //==============================================================================
private:
    // Private member variables
    SynthEngine synthEngine;
    SynthParams& synthParams = { SynthParams::getInstance() };

    std::atomic<float> sinGain = { 0.5f };
    std::atomic<float> squareGain = { 0.5f };
    std::atomic<float> sawGain = { 0.5f };
    std::atomic<float> subSquareGain = { 0.5f };

    std::atomic<float> attack = { 0.5f };
    std::atomic<float> decay = { 0.5f };
    std::atomic<float> sustain = { 0.5f };
    std::atomic<float> release = { 0.5f };

    std::atomic<float> frequency = { 0.5f };
    std::atomic<float> resonance = { 0.5f };
    std::atomic<float> filterEnvelope = { 0.5f };

    juce::AudioBuffer<float> outputAudio = { NUM_CHANNEL, NUM_SAMPLE };

    // juce::MidiMessage
    // MidiMessage (int byte1, int byte2, int byte3, double timeStamp = 0) noexcept;
    double deltaTime = 0.1; // The unit is [sec]
    std::vector<juce::MidiMessage> notes = {
        { NOTE_ON, C1, VEL_100, 0.0 * deltaTime },
        { NOTE_ON, C1, VEL_OFF, 2.0 * deltaTime },
        { NOTE_ON, D1, VEL_100, 1.0 * deltaTime },
        { NOTE_ON, D1, VEL_OFF, 3.0 * deltaTime },
        { NOTE_ON, E1, VEL_100, 2.0 * deltaTime },
        { NOTE_ON, E1, VEL_OFF, 4.0 * deltaTime },
        { NOTE_ON, F1, VEL_100, 3.0 * deltaTime },
        { NOTE_ON, F1, VEL_OFF, 5.0 * deltaTime },
        { NOTE_ON, G1, VEL_100, 4.0 * deltaTime },
        { NOTE_ON, G1, VEL_OFF, 6.0 * deltaTime },
        { NOTE_ON, A1, VEL_100, 5.0 * deltaTime },
        { NOTE_ON, A1, VEL_OFF, 7.0 * deltaTime },
        { NOTE_ON, B1, VEL_100, 6.0 * deltaTime },
        { NOTE_ON, B1, VEL_OFF, 8.0 * deltaTime },
    };

    // juce::MidiBuffer
    // void addEvent (const MidiMessage& midiMessage, int sampleNumber);
    juce::MidiBuffer inputMidiBuffer;

    //==============================================================================
    // Private method

    static int timeSecToSample (double timeSec, double sampleRate, int numSample)
    {
        int ret = static_cast<int> (timeSec * sampleRate);
        ret = std::clamp (ret, 0, numSample);
        return ret;
    }
};

BENCHMARK_F (SynthEngineFixture, render)
(benchmark::State& state)
{
    for (auto _ : state)
    {
        render();
    }
}

BENCHMARK_MAIN();
