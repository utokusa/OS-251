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
struct SineWaveSound   : public juce::SynthesiserSound
{
    SineWaveSound() {}

    bool appliesToNote    (int) override        { return true; }
    bool appliesToChannel (int) override        { return true; }
};

//==============================================================================
// Synthesizer parameters
// This class is singleton.
class SynthParams
{
public:
    static SynthParams& getInstance()
    {
        static SynthParams instance;
        return instance;
    }
    float getAttack() const
    {
        constexpr float minVal = 0.95;
        constexpr float maxVal = 0.9999;
        return minVal + (*attack) * (maxVal - minVal);
    }
    void setAttackPtr(const std::atomic<float>* _attack)
    {
        attack = _attack;
    }
    float getDecay() const
    {
        constexpr float minVal = 0.9995;
        constexpr float maxVal = 0.99999;
        return minVal + (*decay) * (maxVal - minVal);
    }
    void setDecayPtr(const std::atomic<float>* _decay)
    {
        decay = _decay;
    }
    float getSustain() const
    {
        return *sustain;
    }
    void setSustainPtr(const std::atomic<float>* _sustain)
    {
        sustain = _sustain;
    }
    float getRelease() const
    {
        constexpr float minVal = 0.9995;
        constexpr float maxVal = 0.99999;
        return minVal + (*release) * (maxVal - minVal);
    }
    void setReleasePtr(const std::atomic<float>* _release)
    {
        release = _release;
    }
private:
    const std::atomic<float>* attack;
    const std::atomic<float>* decay;
    const std::atomic<float>* sustain;
    const std::atomic<float>* release;
    SynthParams() {}
};

//==============================================================================
class Envelope
{
    using flnum = double;
    
    enum class State
    {
      OFF, ATTACK, DECAY, SUSTAIN, RELEASE
    };
    
public:
    Envelope ()
    : state(State::OFF),
    sp(SynthParams::getInstance()),
    level(0.0)
    {
    }
    
    void noteOn ()
    {
        state = State::ATTACK;
        constexpr flnum levelNoteStart = MAX_LEVEL * 0.01;
            level = levelNoteStart;
    }
    
    void noteOFf ()
    {
        state = State::RELEASE;
    }
    
    void update ()
    {
        if (state == State::ATTACK)
        {
            constexpr flnum valFinishAttack = MAX_LEVEL * 0.99;
            // Value of sp.getAttack() is around 0.99
            level = level < valFinishAttack ? level * 1.0 / sp.getAttack() : MAX_LEVEL;
            if (level >=  valFinishAttack)
            {
                level = 1.0;
                state = State::DECAY;
            }
            
        }
        else if (state == State::DECAY)
        {
            // Value of sp.getDecay() is around 0.99
            level = level * sp.getDecay();
            flnum sustain = sp.getSustain();
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
            level = level * sp.getRelease();
        }
        else
        {
            assert(false && "Unknow state of envelope");
        }
    }
    
    flnum getLevel () const
    {
        return level;
    }
    
private:
    static constexpr flnum MAX_LEVEL = 1.0;
    State state;
    SynthParams& sp;
    flnum level;
};

//==============================================================================
struct SineWaveVoice   : public juce::SynthesiserVoice
{
    SineWaveVoice() {}

    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<SineWaveSound*> (sound) != nullptr;
    }

    void startNote (int midiNoteNumber, float velocity,
                    juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        currentAngle = 0.0;
        level = velocity * 0.15;
        env.noteOn();

        auto cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber);
        auto cyclesPerSample = cyclesPerSecond / getSampleRate();

        angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;
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

    void pitchWheelMoved (int) override      {}
    void controllerMoved (int, int) override {}

    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override
    {
        if (angleDelta != 0.0)
        {
            while (--numSamples >= 0)
            {
                auto currentSample = (float) (std::sin (currentAngle) * level * env.getLevel());

                for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                    outputBuffer.addSample (i, startSample, currentSample);

                currentAngle += angleDelta;
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
    double currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    Envelope env;
};

//==============================================================================
class SynthAudioSource
{
public:
    SynthAudioSource ()
    {
        for (auto i = 0; i < 4; ++i)
            synth.addVoice (new SineWaveVoice());

        synth.addSound (new SineWaveSound());
    }

    void setUsingSineWaveSound()
    {
        synth.clearSounds();
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        midiCollector.reset (sampleRate); // [10]
    }

    void releaseResources() {}
    
    void renderNextBlock (juce::AudioBuffer<float>& outputAudio, const juce::MidiBuffer& inputMidi,
                                       int startSample, int numSamples)
    {
        synth.renderNextBlock (outputAudio, inputMidi, startSample, numSamples);
    }

    juce::MidiMessageCollector* getMidiCollector()
    {
        return &midiCollector;
    }

private:
    juce::Synthesiser synth;
    juce::MidiMessageCollector midiCollector;
};
