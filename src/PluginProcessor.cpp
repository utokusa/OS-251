/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "adapters/JuceAudioBuffer.h"
#include "services/TmpFileManager.h"

//==============================================================================
Os251AudioProcessor::Os251AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
                          .withInput ("Input", juce::AudioChannelSet::stereo(), true)
#endif
                          .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
#endif
                          ),
#endif
      parameters (*this, nullptr),
      synthParams(),
      positionInfo(),
      jucePositionInfo (&positionInfo),
      lfo (synthParams.lfo(), &jucePositionInfo),
      voices (onsen::FancySynthVoice::buildVoices (onsen::SynthEngine::getMaxNumVoices(), &synthParams, &lfo)),
      synth (&synthParams, &jucePositionInfo, &lfo, voices),
      synthEngineAdapter (synth),
      synthUi (synth),
      processorState (parameters),
      presetManager (
          &processorState,
          juce::File::getSpecialLocation (
              juce::File::SpecialLocationType::userApplicationDataDirectory)
              .getChildFile ("Onsen Audio/OS-251/presets")),
      laf()
{
    // Set audio parameters
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    juce::NormalisableRange<float> nrange (0.0, 1.0);

    // Initialize parameters
    auto paramsMetaList = synthParams.getParamMetaList();
    for (auto& p : paramsMetaList)
    {
        parameters.createAndAddParameter (std::make_unique<Parameter> (
            p.paramId, p.paramName, "", nrange, p.defaultValue, [p] (float val) { return juce::String ((p.valueToString) (val)); }, nullptr, true));
        *(p.valuePtr) = parameters.getRawParameterValue (p.paramId);
        parameters.addParameterListener (p.paramId, this);
    }
    synthParams.parameterChanged();

    // ---

    // Parameters that are not kept in `synthParams`.
    // They are changed through functions like `SynthEngineAdapter::changeNumberOfVoices()` or
    // `SynthEngineAdapter::changeIsUnison()`.

    // Number of voices
    auto numVoicesBMI = onsen::OscillatorParams::numVoicesParamBasicMetaInfo();
    parameters.createAndAddParameter (std::make_unique<Parameter> (
        numVoicesBMI.paramId, numVoicesBMI.paramName, "", nrange, numVoicesBMI.defaultValue, numVoicesBMI.valueToString, nullptr, true));
    parameters.addParameterListener (numVoicesBMI.paramId, this);
    synthEngineAdapter.changeNumberOfVoices (onsen::OscillatorParams::convertParamValueToNumVoices (numVoicesBMI.defaultValue));

    // Unison On
    auto unisonOnBMI = onsen::OscillatorParams::unisonOnValueBasicMetaInfo();
    parameters.createAndAddParameter (std::make_unique<Parameter> (
        unisonOnBMI.paramId, unisonOnBMI.paramName, "", nrange, unisonOnBMI.defaultValue, unisonOnBMI.valueToString, nullptr, true));
    parameters.addParameterListener (unisonOnBMI.paramId, this);
    synthEngineAdapter.changeIsUnison (onsen::OscillatorParams::convertParamValueToUnisonOn (unisonOnBMI.defaultValue));

    parameters.state = juce::ValueTree (juce::Identifier ("OS-251"));

    // Preset management
    juce::ValueTree preset (juce::Identifier ("CurrentPreset"));
    preset.setProperty (juce::Identifier ("path"), "Default.oapreset", nullptr);
    parameters.state.addChild (preset, 0, nullptr);

    // Try to load default preset file so that users can
    // have their own default preset.
    presetManager.loadPreset (presetManager.getDefaultPresetFile());
}

Os251AudioProcessor::~Os251AudioProcessor()
{
    onsen::TmpFileManager::cleanUpTmpDir (onsen::TmpFileManager::getTmpDir());
}

//==============================================================================
const juce::String Os251AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool Os251AudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool Os251AudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool Os251AudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double Os251AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int Os251AudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
        // so this should be at least 1, even if you're not really implementing programs.
}

int Os251AudioProcessor::getCurrentProgram()
{
    return 0;
}

void Os251AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String Os251AudioProcessor::getProgramName (int index)
{
    return {};
}

void Os251AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void Os251AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    synthEngineAdapter.prepareToPlay (samplesPerBlock, sampleRate);
    synthParams.prepareToPlay (samplesPerBlock, sampleRate);
}

void Os251AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    synthEngineAdapter.releaseResources();
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool Os251AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}
#endif

void Os251AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    // Host information
    auto playHead = getPlayHead();
    if (playHead)
    {
        // Update positionInfo which jucePositionInfo has its pointer
        playHead->getCurrentPosition (positionInfo);
    }

    // Audio
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        [[maybe_unused]] auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        buffer.clear (channel, 0, buffer.getNumSamples());
    }

    onsen::JuceAudioBuffer audioBuffer (&buffer);
    synthEngineAdapter.renderNextBlock (&audioBuffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Os251AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Os251AudioProcessor::createEditor()
{
    // Look and feel
    juce::LookAndFeel::setDefaultLookAndFeel (&laf);
    return new Os251AudioProcessorEditor (*this, parameters, presetManager, &synthUi);
}

//==============================================================================
void Os251AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void Os251AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (parameters.state.getType()))
        {
            auto newState = juce::ValueTree::fromXml (*xmlState);
            newState = onsen::PresetManager::fixProcessorState (newState);
            parameters.replaceState (newState);
            presetManager.requireToUpdatePresetNameOnUI();
        }
}

void Os251AudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    // TODO: Update parameters in an efficient way (Stop updating all of the parameters)
    synthParams.parameterChanged();

    if (parameterID == "numVoices")
    {
        synthEngineAdapter.changeNumberOfVoices (onsen::OscillatorParams::convertParamValueToNumVoices (newValue));
    }
    else if (parameterID == "unisonOn")
    {
        synthEngineAdapter.changeIsUnison (onsen::OscillatorParams::convertParamValueToUnisonOn (newValue));
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Os251AudioProcessor();
}
