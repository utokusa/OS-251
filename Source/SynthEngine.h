/*
  ==============================================================================

    SynthEngine.h
    Created: 14 Oct 2020 10:23:47pm
    Author:  Utokusa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
struct FancySynthSound : public juce::SynthesiserSound
{
    FancySynthSound() = default;

    bool appliesToNote (int) override { return true; }
    bool appliesToChannel (int) override { return true; }
};

//==============================================================================
class OscillatorParams
{
public:
    float getSinGain() const
    {
        auto decibelGain = paramValToDecibel (*sinGain);
        return decibelToLinear (decibelGain);
    }
    void setSinGainPtr (const std::atomic<float>* _sinGain)
    {
        sinGain = _sinGain;
    }
    float getSquareGain() const
    {
        auto decibelGain = paramValToDecibel (*squareGain);
        return decibelToLinear (decibelGain);
    }
    void setSquareGainPtr (const std::atomic<float>* _squareGain)
    {
        squareGain = _squareGain;
    }
    float getSawGain() const
    {
        auto decibelGain = paramValToDecibel (*sawGain);
        return decibelToLinear (decibelGain);
    }
    void setSawGainPtr (const std::atomic<float>* _sawGain)
    {
        sawGain = _sawGain;
    }

private:
    // Dynamic range in [db]
    static constexpr float dynamicRange = 48.0;
    const std::atomic<float>* sinGain {};
    const std::atomic<float>* squareGain {};
    const std::atomic<float>* sawGain {};

    // Convert parameter value (linear) to gain ([db])
    // in order to make UX better.
    static float paramValToDecibel (float paramVal)
    {
        // e.g.
        // paramVal: 1.0 ---> gain: 0 [db] (max)
        // paramVal: 0.0 ---> gain: -dynamicRange [db] (min)
        return dynamicRange * (paramVal - 1.0);
    }

    static float decibelToLinear (float decibelGain)
    {
        return std::pow (10.f, decibelGain / 20.f);
    }
};

//==============================================================================
class EnvelopeParams
{
public:
    float getAttack() const
    {
        constexpr float minVal = 0.995;
        constexpr float maxVal = 0.99999;
        return minVal + (*attack) * (maxVal - minVal);
    }
    void setAttackPtr (const std::atomic<float>* _attack)
    {
        attack = _attack;
    }
    float getDecay() const
    {
        constexpr float minVal = 0.9995;
        constexpr float maxVal = 0.99999;
        return minVal + (*decay) * (maxVal - minVal);
    }
    void setDecayPtr (const std::atomic<float>* _decay)
    {
        decay = _decay;
    }
    float getSustain() const
    {
        return *sustain;
    }
    void setSustainPtr (const std::atomic<float>* _sustain)
    {
        sustain = _sustain;
    }
    float getRelease() const
    {
        constexpr float minVal = 0.995;
        constexpr float maxVal = 0.99999;
        return minVal + (*release) * (maxVal - minVal);
    }
    void setReleasePtr (const std::atomic<float>* _release)
    {
        release = _release;
    }

private:
    const std::atomic<float>* attack {};
    const std::atomic<float>* decay {};
    const std::atomic<float>* sustain {};
    const std::atomic<float>* release {};
};

class FilterParams
{
public:
    float getFrequency() const
    {
        return lowestFreqVal() * pow (freqBaseNumber(), *frequency);
    }
    void setFrequencyPtr (const std::atomic<float>* _frequency)
    {
        frequency = _frequency;
    }
    float getResonance() const
    {
        return lowestResVal() * pow (resBaseNumber(), *resonance);
    }
    void setResonancePtr (const std::atomic<float>* _resonance)
    {
        resonance = _resonance;
    }
    float getFilterEnvelope() const
    {
        return *filterEnvelope;
    }
    void setFilterEnvelopePtr (const std::atomic<float>* _filterEnvelope)
    {
        filterEnvelope = _filterEnvelope;
    }

    // ---
    // Parameter converting consts
    // Frequency
    static constexpr float lowestFreqVal()
    {
        return 20.0f;
    }
    static constexpr float freqBaseNumber()
    {
        return 1000.0f;
    }
    // Resonance
    static constexpr float lowestResVal()
    {
        return 0.2f;
    }
    static constexpr float resBaseNumber()
    {
        return 100.0;
    }

private:
    const std::atomic<float>* frequency {};
    const std::atomic<float>* resonance {};
    const std::atomic<float>* filterEnvelope {};
};

// Synthesizer parameters
// This class is singleton.
class SynthParams
{
public:
    // Singleton
    SynthParams (const SynthParams&) = delete;
    SynthParams& operator= (SynthParams const&) = delete;
    static SynthParams& getInstance()
    {
        static SynthParams instance;
        return instance;
    }
    EnvelopeParams* envelope()
    {
        return &envelopeParams;
    }
    FilterParams* filter()
    {
        return &filterParams;
    }
    OscillatorParams* oscillator()
    {
        return &oscillatorParams;
    }

private:
    // plugin parameters
    EnvelopeParams envelopeParams;
    FilterParams filterParams;
    OscillatorParams oscillatorParams;
    SynthParams() = default;
};

//==============================================================================
class Oscillator
{
public:
    Oscillator() : p (SynthParams::getInstance().oscillator()) {}

    // Return oscillator voltage value.
    // Angle is in radian.
    double oscillatorVal (double angle)
    {
        auto currentSample = 0.0;
        currentSample += sinWave (angle) * p->getSinGain();
        currentSample += squareWave (angle) * p->getSquareGain();
        currentSample += sawWave (angle) * p->getSawGain();
        return currentSample;
    }

private:
    static constexpr double pi = juce::MathConstants<double>::pi;

    const OscillatorParams* const p;

    static double sinWave (double angle)
    {
        return std::sin (angle);
    }

    static double squareWave (double angle)
    {
        return angle < pi ? 1.0 : -1.0;
    }

    static double sawWave (double angle)
    {
        return std::min (2.0 * angle / (2.0 * pi), 2.0) - 1.0;
    }
};

//==============================================================================
class Envelope
{
    using flnum = double;

    enum class State
    {
        OFF,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };

public:
    Envelope()
        : p (SynthParams::getInstance().envelope()),
          sampleRate (DEFAULT_SAMPLE_RATE),
          state (State::OFF),
          level (0.0)
    {
    }

    void noteOn()
    {
        state = State::ATTACK;
        constexpr flnum levelNoteStart = MAX_LEVEL * 0.01;
        level = levelNoteStart;
    }

    void noteOFf()
    {
        state = State::RELEASE;
    }

    void update()
    {
        if (state == State::ATTACK)
        {
            constexpr flnum valFinishAttack = MAX_LEVEL * 0.99;
            // Value of sp.getAttack() is around 0.99
            level = level * MAX_LEVEL / adjust (p->getAttack());
            if (level >= valFinishAttack)
            {
                level = MAX_LEVEL;
                state = State::DECAY;
            }
        }
        else if (state == State::DECAY)
        {
            // Value of sp.getDecay() is around 0.99
            level = level * adjust (p->getDecay());
            flnum sustain = p->getSustain();
            if (level <= sustain)
            {
                level = sustain;
                state = State::SUSTAIN;
            }
        }
        else if (state == State::SUSTAIN)
        {
            // Nothing to do here
        }
        else if (state == State::RELEASE)
        {
            // Value of sp.getRelease() is around 0.99
            level = level * adjust (p->getRelease());
        }
        else
        {
            assert (false && "Unknown state of envelope");
        }
    }

    flnum getLevel() const
    {
        return level;
    }

    void setCurrentPlaybackSampleRate (const double newRate)
    {
        sampleRate = newRate;
    }

private:
    static constexpr flnum MAX_LEVEL = 1.0;
    static constexpr flnum DEFAULT_SAMPLE_RATE = 44100.0;
    static constexpr flnum EPSILON = std::numeric_limits<flnum>::epsilon();

    const EnvelopeParams* const p;
    flnum sampleRate;

    State state;
    flnum level;

    // Adjust parameter value like attack, decay or release according to the
    // sampling rate
    flnum adjust (const flnum val) const
    {
        // If no need to adjust
        if (std::abs (sampleRate - DEFAULT_SAMPLE_RATE) <= EPSILON)
        {
            return val;
        }
        flnum amount = std::pow (val, DEFAULT_SAMPLE_RATE / sampleRate - 1);
        return val * amount;
    }
};

//==============================================================================
class Filter
{
    using flnum = double;
    struct FilterBuffer
    {
    public:
        FilterBuffer() : in1 (0.0f), in2 (0.0f), out1 (0.0f), out2 (0.0f) {}
        ~FilterBuffer() = default;
        ;
        float in1, in2;
        float out1, out2;
    };

public:
    explicit Filter (int _numChannels)
        : p (SynthParams::getInstance().filter()),
          sampleRate (DEFAULT_SAMPLE_RATE),
          numChannels (_numChannels),
          filterBuffers (numChannels)
    {
    }

    void process (juce::AudioBuffer<float>& buffer)
    {
        // Set biquad parameter coefficients
        // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
        flnum omega0 = 2.0f * 3.14159265f * p->getFrequency() / sampleRate;
        flnum sinw0 = std::sin (omega0);
        flnum cosw0 = std::cos (omega0);
        // sp.getResonance() stands for "Q".
        flnum alpha = sinw0 / 2.0 / p->getResonance();
        flnum a0 = 1.0 + alpha;
        flnum a1 = -2.0 * cosw0;
        flnum a2 = 1.0 - alpha;
        flnum b0 = (1 - cosw0) / 2.0;
        flnum b1 = 1 - cosw0;
        flnum b2 = (1 - cosw0) / 2.0;

        // Calculate
        int numInputChannels = buffer.getNumChannels();
        int bufferSize = buffer.getNumSamples();
        for (int channel = 0; channel < std::min (numChannels, numInputChannels); channel++)
        {
            FilterBuffer& fb = filterBuffers[channel];
            float* bufferPtr = buffer.getWritePointer (channel);
            for (int i = 0; i < bufferSize; i++)
            {
                flnum out0 = b0 / a0 * bufferPtr[i] + b1 / a0 * fb.in1 + b2 / a0 * fb.in2
                             - a1 / a0 * fb.out1 - a2 / a0 * fb.out2;
                fb.in2 = fb.in1;
                fb.in1 = bufferPtr[i];

                fb.out2 = fb.out1;
                fb.out1 = out0;

                bufferPtr[i] = out0;
            }
        }
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = _sampleRate;
    }

private:
    static constexpr flnum DEFAULT_SAMPLE_RATE = 44100.0;

    const FilterParams* const p;
    flnum sampleRate;
    int numChannels;
    // The length of this vector equals to max number of the channels;
    std::vector<FilterBuffer> filterBuffers;
};

//==============================================================================
struct FancySynthVoice : public juce::SynthesiserVoice
{
    FancySynthVoice() = default;

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<FancySynthSound*> (sound) != nullptr;
    }

    void setCurrentPlaybackSampleRate (const double newRate) override
    {
        juce::SynthesiserVoice::setCurrentPlaybackSampleRate (newRate);
        env.setCurrentPlaybackSampleRate (newRate);
    }

    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        env.noteOn();

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * pi;
    }

    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        if (allowTailOff)
        {
            env.noteOFf();
        }
        else
        {
            clearCurrentNote();
            angleDelta = 0.0;
        }
    }

    void pitchWheelMoved (int) override {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            while (--numSamples >= 0)
            {
                auto currentSample = (float) (osc.oscillatorVal (currentAngle) * level * env.getLevel());

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);

                currentAngle += angleDelta;
                if (currentAngle > pi * 2.0)
                {
                    currentAngle -= pi * 2.0;
                }
                ++startSample;
                env.update();
                if (env.getLevel() < 0.005)
                {
                    clearCurrentNote();
                    angleDelta = 0.0;
                    break;
                }
            }
        }
    }

private:
    static constexpr double pi = juce::MathConstants<double>::pi;
    // We use angle in radian
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    Oscillator osc;
    Envelope env;
};

//==============================================================================
class SynthEngine
{
public:
    explicit SynthEngine (int numChannels)
        : filter (numChannels)
    {
        for (auto i = 0; i < 4; ++i)
            synth.addVoice (new FancySynthVoice());

        synth.addSound (new FancySynthSound());
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        filter.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate);
    }

    void releaseResources() {}

    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        synth.renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
        filter.process (outputAudio);
    }

    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }

private:
    juce::Synthesiser synth;
    Filter filter;
    juce::MidiMessageCollector midiCollector;
};
