/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "JuceAudioProcessorState.h"
#include "adapters/JucePositionInfo.h"
#include "adapters/JuceSynthEngineAdapter.h"
#include "services/PresetManager.h"
#include "synth/SynthUi.h"
#include "views/GlobalLookAndFeel.h"
#include <JuceHeader.h>

//==============================================================================
/**
*/
class Os251AudioProcessor : public juce::AudioProcessor, juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    Os251AudioProcessor();
    ~Os251AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
#endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState parameters;
    onsen::SynthParams synthParams;
    juce::AudioPlayHead::CurrentPositionInfo positionInfo;
    onsen::JucePositionInfo jucePositionInfo;
    onsen::Lfo lfo;
    std::vector<std::shared_ptr<onsen::ISynthVoice>> voices;
    onsen::SynthEngine synth;
    onsen::JuceSynthEngineAdapter synthEngineAdapter;
    onsen::SynthUi synthUi;
    onsen::JuceAudioProcessorState processorState;
    onsen::PresetManager presetManager;
    onsen::GlobalLookAndFeel laf;

    //==============================================================================
    void parameterChanged (const juce::String& parameterID, float newValue) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessor)
};
