/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
      synthEngine()
{
    SynthParams& synthParams (SynthParams::getInstance());

    // ---
    // Parameter value conversion from [0, 1.0] float to juce::String.

    // Map [0, 1.0] to ["0", "0.1"]
    constexpr int numDecimal = 4;
    auto valueToTextFunction = [] (float value) { return juce::String (value, numDecimal); };

    // Frequency
    constexpr float lowestFreqVal = FilterParams::lowestFreqVal();
    constexpr float freqBaseNumber = FilterParams::freqBaseNumber();
    auto valueToFreqFunction = [] (float value) { return juce::String ((int) (pow (freqBaseNumber, value) * lowestFreqVal)) + juce::String (" Hz"); };

    // Resonance
    constexpr float lowestResVal = FilterParams::lowestResVal();
    constexpr float resBaseNumber = FilterParams::resBaseNumber();
    auto valueToResFunction = [] (float value) { return juce::String (pow (resBaseNumber, value) * lowestResVal, numDecimal); };
    // ---

    // ---
    // Set audio parameters
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    juce::NormalisableRange<float> nrange (0.0f, 1.0f, 0.01f);
    juce::NormalisableRange<float> nrangeSigned (-1.0f, 1.0f, 0.01f);

    // Oscillator parameters
    OscillatorParams* const oscillatorParams = synthParams.oscillator();

    // Sin gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sinGain", "Sin", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    oscillatorParams->setSinGainPtr (parameters.getRawParameterValue ("sinGain"));
    parameters.addParameterListener ("sinGain", this);

    // Square gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("squareGain", "Square", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    oscillatorParams->setSquareGainPtr (parameters.getRawParameterValue ("squareGain"));
    parameters.addParameterListener ("squareGain", this);

    // Saw gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sawGain", "Saw", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    oscillatorParams->setSawGainPtr (parameters.getRawParameterValue ("sawGain"));
    parameters.addParameterListener ("sawGain", this);

    // Sub square gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("subSquareGain", "SubSquare", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    oscillatorParams->setSubSquareGainPtr (parameters.getRawParameterValue ("subSquareGain"));
    parameters.addParameterListener ("subSquareGain", this);

    // Noise gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("noiseGain", "Noise", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    oscillatorParams->setNoiseGainPtr(parameters.getRawParameterValue ("noiseGain"));
    parameters.addParameterListener ("noiseGain", this);

    // Envelop parameters
    EnvelopeParams* const envelopeParams = synthParams.envelope();

    // Attack
    parameters.createAndAddParameter (std::make_unique<Parameter> ("attack", "Attack", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    envelopeParams->setAttackPtr (parameters.getRawParameterValue ("attack"));
    parameters.addParameterListener ("attack", this);

    // Decay
    parameters.createAndAddParameter (std::make_unique<Parameter> ("decay", "Decay", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    envelopeParams->setDecayPtr (parameters.getRawParameterValue ("decay"));
    parameters.addParameterListener ("decay", this);

    // Sustain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sustain", "Sustain", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    envelopeParams->setSustainPtr (parameters.getRawParameterValue ("sustain"));
    parameters.addParameterListener ("sustain", this);

    // Release
    parameters.createAndAddParameter (std::make_unique<Parameter> ("release", "Release", "", nrange, 1.0f, valueToTextFunction, nullptr, true));
    envelopeParams->setReleasePtr (parameters.getRawParameterValue ("release"));
    parameters.addParameterListener ("release", this);

    // Filter parameters
    FilterParams* const filterParams = synthParams.filter();

    // Filter cutoff frequency
    parameters.createAndAddParameter (std::make_unique<Parameter> ("frequency", "Frequency", "", nrange, 1.0f, valueToFreqFunction, nullptr, true));
    filterParams->setFrequencyPtr (parameters.getRawParameterValue ("frequency"));
    parameters.addParameterListener ("frequency", this);

    // Filter resonance
    parameters.createAndAddParameter (std::make_unique<Parameter> ("resonance", "Resonance", "", nrange, 1.0f, valueToResFunction, nullptr, true));
    filterParams->setResonancePtr (parameters.getRawParameterValue ("resonance"));
    parameters.addParameterListener ("resonance", this);

    // Filter envelope
    parameters.createAndAddParameter (std::make_unique<Parameter> ("filterEnv", "Filter Env", "", nrangeSigned, 0.0f, valueToTextFunction, nullptr, true));
    filterParams->setFilterEnvelopePtr (parameters.getRawParameterValue (("filterEnv")));
    parameters.addParameterListener ("filterEnv", this);
    // ---

    parameters.state = juce::ValueTree (juce::Identifier ("OS-251"));
}

Os251AudioProcessor::~Os251AudioProcessor() = default;

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
    synthEngine.prepareToPlay (samplesPerBlock, sampleRate);
}

void Os251AudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
    synthEngine.releaseResources();
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
        __unused auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        buffer.clear (channel, 0, buffer.getNumSamples());
    }

    synthEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool Os251AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* Os251AudioProcessor::createEditor()
{
    return new Os251AudioProcessorEditor (*this, parameters);
}

//==============================================================================
void Os251AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void Os251AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void Os251AudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    SynthParams& synthParams (SynthParams::getInstance());
    OscillatorParams* const oscillatorParams = synthParams.oscillator();
    EnvelopeParams* const envelopeParams = synthParams.envelope();
    FilterParams* const filterParams = synthParams.filter();
    oscillatorParams->parameterChanged();
    envelopeParams->parameterChanged();
    filterParams->parameterChanged();

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Os251AudioProcessor();
}
