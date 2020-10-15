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
    const std::atomic<float>* release;
    SynthParams() {}
};

//==============================================================================
class Envelope
{
    using flnum = double;
public:
    Envelope ()
    : on(false),
    sp(SynthParams::getInstance()),
    level(0.0)
    {
    }
    
    void noteOn ()
    {
        on = true;
        level = MAX_LEVEL;
    }
    
    void noteOFf ()
    {
        on = false;
    }
    
    void update ()
    {
        if (on)
            return;
        // Value of sp.getRelease() is around 0.99
        level = level * sp.getRelease();
    }
    
    flnum getLevel () const
    {
        return level;
    }
    
private:
    static constexpr double MAX_LEVEL = 1.0;
    bool on;
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
