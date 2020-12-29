/*
  ==============================================================================

    SynthEngine.h
    Created: 14 Oct 2020 10:23:47pm
    Author:  Utokusa

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <random>

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
    using flnum = float;

public:
    flnum getSinGain() const
    {
        auto decibelGain = paramValToDecibel (sinGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSinGainPtr (const std::atomic<flnum>* _sinGain)
    {
        sinGain = _sinGain;
        sinGainVal = *sinGain;
    }
    flnum getSquareGain() const
    {
        auto decibelGain = paramValToDecibel (squareGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSquareGainPtr (const std::atomic<flnum>* _squareGain)
    {
        squareGain = _squareGain;
        squareGainVal = *squareGain;
    }
    flnum getSawGain() const
    {
        auto decibelGain = paramValToDecibel (sawGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSawGainPtr (const std::atomic<flnum>* _sawGain)
    {
        sawGain = _sawGain;
        sawGainVal = *sawGain;
    }
    flnum getSubSquareGain() const
    {
        auto decibelGain = paramValToDecibel (subSquareGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSubSquareGainPtr (const std::atomic<flnum>* _subSquareGain)
    {
        subSquareGain = _subSquareGain;
        subSquareGainVal = *subSquareGain;
    }
    flnum getNoiseGain() const
    {
        auto decibelGain = paramValToDecibel (noiseGainVal);
        return decibelToLinear (decibelGain);
    }
    void setNoiseGainPtr (const std::atomic<flnum>* _noiseGain)
    {
        noiseGain = _noiseGain;
        noiseGainVal = *noiseGain;
    }
    void parameterChanged()
    {
        sinGainVal = *sinGain;
        squareGainVal = *squareGain;
        sawGainVal = *sawGain;
        subSquareGainVal = *subSquareGain;
        noiseGainVal = *noiseGain;
    }

private:
    // Dynamic range in [db]
    static constexpr flnum dynamicRange = 48.0;
    const std::atomic<flnum>* sinGain {};
    const std::atomic<flnum>* squareGain {};
    const std::atomic<flnum>* sawGain {};
    const std::atomic<flnum>* subSquareGain {};
    const std::atomic<flnum>* noiseGain {};

    flnum sinGainVal = 0.0;
    flnum squareGainVal = 0.0;
    flnum sawGainVal = 0.0;
    flnum subSquareGainVal = 0.0;
    flnum noiseGainVal = 0.0;

    // Convert parameter value (linear) to gain ([db])
    // in order to make UX better.
    static flnum paramValToDecibel (flnum paramVal)
    {
        // e.g.
        // paramVal: 1.0 ---> gain: 0 [db] (max)
        // paramVal: 0.0 ---> gain: -dynamicRange [db] (min)
        return dynamicRange * (paramVal - 1.0);
    }

    static flnum decibelToLinear (flnum decibelGain)
    {
        return std::pow (10.0, decibelGain / 20.0);
    }
};

//==============================================================================
class EnvelopeParams
{
    using flnum = float;
public:
    flnum getAttack() const
    {
        constexpr flnum minVal = 0.995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (attackVal) * (maxVal - minVal);
    }
    void setAttackPtr (const std::atomic<flnum>* _attack)
    {
        attack = _attack;
        attackVal = *attack;
    }
    flnum getDecay() const
    {
        constexpr flnum minVal = 0.9995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (decayVal) * (maxVal - minVal);
    }
    void setDecayPtr (const std::atomic<flnum>* _decay)
    {
        decay = _decay;
        decayVal = *decay;
    }
    flnum getSustain() const
    {
        return sustainVal;
    }
    void setSustainPtr (const std::atomic<flnum>* _sustain)
    {
        sustain = _sustain;
        sustainVal = *sustain;
    }
    flnum getRelease() const
    {
        constexpr flnum minVal = 0.995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (releaseVal) * (maxVal - minVal);
    }
    void setReleasePtr (const std::atomic<flnum>* _release)
    {
        release = _release;
        releaseVal = *release;
    }
    void parameterChanged()
    {
        attackVal = *attack;
        decayVal = *decay;
        sustainVal = *sustain;
        releaseVal = *release;
    }

private:
    const std::atomic<flnum>* attack {};
    const std::atomic<flnum>* decay {};
    const std::atomic<flnum>* sustain {};
    const std::atomic<flnum>* release {};

    flnum attackVal = 0.0;
    flnum decayVal = 0.0;
    flnum sustainVal = 0.0;
    flnum releaseVal = 0.0;
};

class FilterParams
{
    using flnum = float;

public:
    flnum getFrequency() const
    {
        return lowestFreqVal() * pow (freqBaseNumber(), frequencyVal);
    }
    flnum getControlledFrequency (flnum controlVal) const
    {
        flnum newFrequency = std::clamp<flnum> (frequencyVal + controlVal, 0.0, 1.0);
        return lowestFreqVal() * pow (freqBaseNumber(), newFrequency);
    }
    void setFrequencyPtr (const std::atomic<flnum>* _frequency)
    {
        frequency = _frequency;
        frequencyVal = *frequency;
    }
    flnum getResonance() const
    {
        return lowestResVal() * std::pow (resBaseNumber(), resonanceVal);
    }
    void setResonancePtr (const std::atomic<flnum>* _resonance)
    {
        resonance = _resonance;
        resonanceVal = *resonance;
    }
    flnum getFilterEnvelope() const
    {
        return filterEnvelopeVal;
    }
    void setFilterEnvelopePtr (const std::atomic<flnum>* _filterEnvelope)
    {
        filterEnvelope = _filterEnvelope;
        filterEnvelopeVal = *filterEnvelope;
    }
    void parameterChanged()
    {
        frequencyVal = *frequency;
        resonanceVal = *resonance;
        filterEnvelopeVal = *filterEnvelope;
    }

    // ---
    // Parameter converting consts
    // Frequency
    static constexpr flnum lowestFreqVal()
    {
        return 20.0;
    }
    static constexpr flnum freqBaseNumber()
    {
        return 1000.0;
    }
    // Resonance
    static constexpr flnum lowestResVal()
    {
        return 0.2;
    }
    static constexpr flnum resBaseNumber()
    {
        return 100.0;
    }

private:
    const std::atomic<flnum>* frequency {};
    const std::atomic<flnum>* resonance {};
    const std::atomic<flnum>* filterEnvelope {};

    flnum frequencyVal = 0.0;
    flnum resonanceVal = 0.0;
    flnum filterEnvelopeVal = 0.0;
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
    using flnum = float;

public:
    Oscillator() : p (SynthParams::getInstance().oscillator()) {}

    // Return oscillator voltage value.
    // Angle is in radian.
    flnum oscillatorVal (flnum angle)
    {
        flnum currentSample = 0.0;
        currentSample += sinWave (wrapAngle (angle * 2)) * p->getSinGain();
        currentSample += squareWave (wrapAngle (angle * 2)) * p->getSquareGain();
        currentSample += sawWave (wrapAngle (angle * 2)) * p->getSawGain();
        currentSample += squareWave (angle) * p->getSubSquareGain();
        currentSample += noiseWave() * p->getNoiseGain();
        return currentSample;
    }

private:
    static constexpr flnum pi = juce::MathConstants<flnum>::pi;

    const OscillatorParams* const p;

    static flnum wrapAngle (flnum angle)
    {
        while (angle > 2 * pi)
        {
            angle -= 2 * pi;
        }
        return angle;
    }

    static flnum sinWave (flnum angle)
    {
        return std::sin (angle);
    }

    static flnum squareWave (flnum angle)
    {
        return angle < pi ? 1.0 : -1.0;
    }

    static flnum sawWave (flnum angle)
    {
        return std::min (2.0 * angle / (2.0 * pi), 2.0) - 1.0;
    }

    static flnum noiseWave()
    {
        static std::random_device seedGen;
        static std::default_random_engine engine (seedGen());
        static std::uniform_real_distribution<> dist (0.0, 1.0);
        return dist (engine);
    }
};

//==============================================================================
class Envelope
{
    using flnum = float;

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
    using flnum = float;
    struct FilterBuffer
    {
    public:
        FilterBuffer() : in1 (0.0), in2 (0.0), out1 (0.0), out2 (0.0) {}
        ~FilterBuffer() = default;
        ;
        flnum in1, in2;
        flnum out1, out2;
    };

public:
    explicit Filter (Envelope& _env)
        : p (SynthParams::getInstance().filter()),
          env (_env),
          sampleRate (DEFAULT_SAMPLE_RATE),
          fb()
    {
    }

    flnum process (flnum sample)
    {
        // Set biquad parameter coefficients
        // https://webaudio.github.io/Audio-EQ-Cookbook/audio-eq-cookbook.html
        flnum omega0 = 2.0 * pi * p->getControlledFrequency (env.getLevel() * p->getFilterEnvelope()) / sampleRate;
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

        flnum out0 = b0 / a0 * sample + b1 / a0 * fb.in1 + b2 / a0 * fb.in2
                     - a1 / a0 * fb.out1 - a2 / a0 * fb.out2;
        fb.in2 = fb.in1;
        fb.in1 = sample;

        fb.out2 = fb.out1;
        fb.out1 = out0;

        return out0;
    }

    void setCurrentPlaybackSampleRate (double _sampleRate)
    {
        sampleRate = static_cast<flnum> (_sampleRate);
    }

private:
    static constexpr flnum DEFAULT_SAMPLE_RATE = 44100.0;
    static constexpr flnum pi = juce::MathConstants<flnum>::pi;

    const FilterParams* const p;
    Envelope& env;
    flnum sampleRate;
    // The length of this vector equals to max number of the channels;
    FilterBuffer fb;
};

//==============================================================================
struct FancySynthVoice : public juce::SynthesiserVoice
{
    using flnum = float;
    FancySynthVoice()
        : env(),
          filter (env)
    {
    }

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<FancySynthSound*> (sound) != nullptr;
    }

    void setCurrentPlaybackSampleRate (const double newRate) override
    {
        juce::SynthesiserVoice::setCurrentPlaybackSampleRate (newRate);
        env.setCurrentPlaybackSampleRate (newRate);
        filter.setCurrentPlaybackSampleRate (newRate);
    }

    void startNote (int midiNoteNumber, flnum velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        env.noteOn();

        flnum adjustOctave = 2.0;
        flnum cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber) / adjustOctave;
        flnum cyclesPerSample = cyclesPerSecond / getSampleRate();

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
                flnum currentSample = osc.oscillatorVal (currentAngle) * level * env.getLevel();
                currentSample = filter.process (currentSample);

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
    static constexpr flnum pi = juce::MathConstants<flnum>::pi;
    // We use angle in radian
    flnum currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    Oscillator osc;
    Envelope env;
    Filter filter;
};

//==============================================================================
class SynthEngine
{
public:
    SynthEngine()
    {
        for (auto i = 0; i < 4; ++i)
            synth.addVoice (new FancySynthVoice());

        synth.addSound (new FancySynthSound());
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate);
    }

    void releaseResources() {}

    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        synth.renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
    }

private:
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
};
