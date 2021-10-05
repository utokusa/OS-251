/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
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
      synthEngine (&synthParams, &jucePositionInfo),
      processorState (parameters),
      presetManager (
          &processorState,
          juce::File::getSpecialLocation (
              juce::File::SpecialLocationType::userApplicationDataDirectory)
              .getChildFile ("Onsen Audio/OS-251/presets")),
      laf()
{
    // ---
    // Parameter value conversion from [0, 1.0] float to juce::String.

    // Map [0, 1.0] to ["0", "0.1"]
    constexpr int numDecimal = 4;
    auto valueToTextFunction = [numDecimal] (float value) { return juce::String (value, numDecimal); };
    auto valueToMinusOneToOneFunction = [numDecimal] (float value) { return juce::String (onsen::DspUtil::valMinusOneToOne (value), numDecimal); };

    // Synced Rate
    auto syncedRateTextFunction = [] (float value) { return juce::String (
                                                                onsen::DspUtil::mapFlnumToInt (value, 0.0, 1.0, onsen::LfoParams::lowestRateSyncNumeratorVal(), onsen::LfoParams::highestRateSyncNumeratorVal(), true))
                                                            + juce::String ("/")
                                                            + juce::String (onsen::LfoParams::rateSyncDenominatorVal()); };

    // Frequency
    constexpr float lowestFreqVal = onsen::FilterParams::lowestFreqVal();
    constexpr float freqBaseNumber = onsen::FilterParams::freqBaseNumber();
    auto valueToFreqFunction = [lowestFreqVal, freqBaseNumber] (float value) { return juce::String ((int) (pow (freqBaseNumber, value) * lowestFreqVal)) + juce::String (" Hz"); };

    // Resonance
    constexpr float lowestResVal = onsen::FilterParams::lowestResVal();
    constexpr float resBaseNumber = onsen::FilterParams::resBaseNumber();
    auto valueToResFunction = [lowestResVal, resBaseNumber, numDecimal] (float value) { return juce::String (pow (resBaseNumber, value) * lowestResVal, numDecimal); };

    // ON / OFF
    auto valueToOnOff = [] (float value) { return value > 0.5 ? juce::String ("ON") : juce::String ("OFF"); };

    // Master octave tuning
    auto pitchBendWidtValToStr = [] (float value) { return juce::String (
                                                        onsen::DspUtil::mapFlnumToInt (
                                                            value, 0.0, 1.0, 0, onsen::MasterParams::maxPitchBendWidth)); };
    auto masterOctaveTuningValToStr = [] (float value) { return juce::String (
                                                             onsen::DspUtil::mapFlnumToInt (value, 0.0, 1.0, -onsen::MasterParams::maxOctaveTuneVal, onsen::MasterParams::maxOctaveTuneVal)); };
    auto masterSemitoneTuningValToStr = [] (float value) { return juce::String (
                                                                      onsen::DspUtil::mapFlnumToInt (value, 0.0, 1.0, -onsen::MasterParams::maxSemitoneTuneVal, onsen::MasterParams::maxSemitoneTuneVal))
                                                                  + juce::String (" st"); };

    // Number of voices
    auto numVoicesToStr = [] (float value) { return juce::String (
                                                 onsen::DspUtil::mapFlnumToInt (
                                                     value, 0.0, 1.0, 1, onsen::MasterParams::maxNumVoices)); };
    // ---

    // ---
    // Set audio parameters
    using Parameter = juce::AudioProcessorValueTreeState::Parameter;
    juce::NormalisableRange<float> nrange (0.0, 1.0);

    // Oscillator parameters
    onsen::OscillatorParams* const oscillatorParams = synthParams.oscillator();

    // Sin gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sinGain", "Sin", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setSinGainPtr (parameters.getRawParameterValue ("sinGain"));
    parameters.addParameterListener ("sinGain", this);

    // Square gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("squareGain", "Square", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setSquareGainPtr (parameters.getRawParameterValue ("squareGain"));
    parameters.addParameterListener ("squareGain", this);

    // Saw gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sawGain", "Saw", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setSawGainPtr (parameters.getRawParameterValue ("sawGain"));
    parameters.addParameterListener ("sawGain", this);

    // Sub square gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("subSquareGain", "SubSquare", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setSubSquareGainPtr (parameters.getRawParameterValue ("subSquareGain"));
    parameters.addParameterListener ("subSquareGain", this);

    // Noise gain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("noiseGain", "Noise", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setNoiseGainPtr (parameters.getRawParameterValue ("noiseGain"));
    parameters.addParameterListener ("noiseGain", this);

    // Noise shape
    parameters.createAndAddParameter (std::make_unique<Parameter> ("shape", "Shape", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    oscillatorParams->setShapePtr (parameters.getRawParameterValue ("shape"));
    parameters.addParameterListener ("shape", this);

    // Envelop parameters
    onsen::EnvelopeParams* const envelopeParams = synthParams.envelope();

    // Attack
    parameters.createAndAddParameter (std::make_unique<Parameter> ("attack", "Attack", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    envelopeParams->setAttackPtr (parameters.getRawParameterValue ("attack"));
    parameters.addParameterListener ("attack", this);

    // Decay
    parameters.createAndAddParameter (std::make_unique<Parameter> ("decay", "Decay", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    envelopeParams->setDecayPtr (parameters.getRawParameterValue ("decay"));
    parameters.addParameterListener ("decay", this);

    // Sustain
    parameters.createAndAddParameter (std::make_unique<Parameter> ("sustain", "Sustain", "", nrange, 1.0, valueToTextFunction, nullptr, true));
    envelopeParams->setSustainPtr (parameters.getRawParameterValue ("sustain"));
    parameters.addParameterListener ("sustain", this);

    // Release
    parameters.createAndAddParameter (std::make_unique<Parameter> ("release", "Release", "", nrange, 0.5, valueToTextFunction, nullptr, true));
    envelopeParams->setReleasePtr (parameters.getRawParameterValue ("release"));
    parameters.addParameterListener ("release", this);

    // LFO parameters
    onsen::LfoParams* const lfoParams = synthParams.lfo();

    // LFO rate
    parameters.createAndAddParameter (std::make_unique<Parameter> ("rate", "LFO Rate", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    lfoParams->setRatePtr (parameters.getRawParameterValue ("rate"));
    parameters.addParameterListener ("rate", this);

    // LFO rateSync
    parameters.createAndAddParameter (std::make_unique<Parameter> ("rateSync", "Synced LFO Rate", "", nrange, 0.0, syncedRateTextFunction, nullptr, true));
    lfoParams->setRateSyncPtr (parameters.getRawParameterValue ("rateSync"));
    parameters.addParameterListener ("rateSync", this);

    // Lfo phase
    parameters.createAndAddParameter (std::make_unique<Parameter> ("lfoPhase", "LFO Phase", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    lfoParams->setPhasePtr (parameters.getRawParameterValue (("lfoPhase")));
    parameters.addParameterListener ("lfoPhase", this);

    // LFO delay
    parameters.createAndAddParameter (std::make_unique<Parameter> ("lfoDelay", "LFO Chorus", "", nrange, 0.5, valueToTextFunction, nullptr, true));
    lfoParams->setDelayPtr (parameters.getRawParameterValue ("lfoDelay"));
    parameters.addParameterListener ("lfoDelay", this);

    // Sync On
    parameters.createAndAddParameter (std::make_unique<Parameter> ("syncOn", "Sync", "", nrange, 0.0, valueToOnOff, nullptr, true));
    lfoParams->setSyncOnPtr (parameters.getRawParameterValue (("syncOn")));
    parameters.addParameterListener ("syncOn", this);

    // Amount of LFO pitch
    parameters.createAndAddParameter (std::make_unique<Parameter> ("lfoPitch", "LFO -> Pitch", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    lfoParams->setPitchPtr (parameters.getRawParameterValue ("lfoPitch"));
    parameters.addParameterListener ("lfoPitch", this);

    // Amount of LFO filter cutoff frequency
    parameters.createAndAddParameter (std::make_unique<Parameter> ("lfoFilterFreq", "LFO -> Freq", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    lfoParams->setFilterFreqPtr (parameters.getRawParameterValue ("lfoFilterFreq"));
    parameters.addParameterListener ("lfoFilterFreq", this);

    // Amount of LFO oscillator shape
    parameters.createAndAddParameter (std::make_unique<Parameter> ("lfoShape", "LFO -> Shape", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    lfoParams->setShapePtr (parameters.getRawParameterValue ("lfoShape"));
    parameters.addParameterListener ("lfoShape", this);

    // Filter parameters
    onsen::FilterParams* const filterParams = synthParams.filter();

    // Filter cutoff frequency
    parameters.createAndAddParameter (std::make_unique<Parameter> ("frequency", "Frequency", "", nrange, 1.0, valueToFreqFunction, nullptr, true));
    filterParams->setFrequencyPtr (parameters.getRawParameterValue ("frequency"));
    parameters.addParameterListener ("frequency", this);

    // Filter resonance
    parameters.createAndAddParameter (std::make_unique<Parameter> ("resonance", "Resonance", "", nrange, 0.35 /* converted to 1.0024*/, valueToResFunction, nullptr, true));
    filterParams->setResonancePtr (parameters.getRawParameterValue ("resonance"));
    parameters.addParameterListener ("resonance", this);

    // Filter envelope
    parameters.createAndAddParameter (std::make_unique<Parameter> ("filterEnv", "Env -> Filter", "", nrange, 0.5, valueToTextFunction, nullptr, true));
    filterParams->setFilterEnvelopePtr (parameters.getRawParameterValue (("filterEnv")));
    parameters.addParameterListener ("filterEnv", this);

    // HPF parameters
    onsen::HpfParams* const hpfParams = synthParams.hpf();
    parameters.createAndAddParameter (std::make_unique<Parameter> ("hpfFreq", "HPF Freq", "", nrange, 0.0, valueToFreqFunction, nullptr, true));
    hpfParams->setFrequencyPtr (parameters.getRawParameterValue ("hpfFreq"));
    parameters.addParameterListener ("hpfFreq", this);

    // Chorus parameters
    onsen::ChorusParams* const chorusParams = synthParams.chorus();

    // Chorus ON
    parameters.createAndAddParameter (std::make_unique<Parameter> ("chorusOn", "Chorus", "", nrange, 0.0, valueToOnOff, nullptr, true));
    chorusParams->setChorusOnPtr (parameters.getRawParameterValue (("chorusOn")));
    parameters.addParameterListener ("chorusOn", this);

    // Master parameters
    onsen::MasterParams* const masterParams = synthParams.master();

    // Env/Gate switch for amplitude
    parameters.createAndAddParameter (std::make_unique<Parameter> ("envForAmpOn", "Env -> Amp", "", nrange, 1.0, valueToOnOff, nullptr, true));
    masterParams->setEnvForAmpOnPtr (parameters.getRawParameterValue (("envForAmpOn")));
    parameters.addParameterListener ("envForAmpOn", this);

    // Pitch bend width
    parameters.createAndAddParameter (std::make_unique<Parameter> ("pitchBendWidth", "Pitch Bend", "", nrange, 0.5, pitchBendWidtValToStr, nullptr, true));
    masterParams->setPitchBendWidthPtr (parameters.getRawParameterValue (("pitchBendWidth")));
    parameters.addParameterListener ("pitchBendWidth", this);

    // Master octave tuning
    parameters.createAndAddParameter (std::make_unique<Parameter> ("masterOctaveTune", "Octave", "", nrange, 0.5, masterOctaveTuningValToStr, nullptr, true));
    masterParams->setMasterOctaveTunePtr (parameters.getRawParameterValue (("masterOctaveTune")));
    parameters.addParameterListener ("masterOctaveTune", this);

    // Master semitone tuning
    parameters.createAndAddParameter (std::make_unique<Parameter> ("masterSemitoneTune", "Semi", "", nrange, 0.5, masterSemitoneTuningValToStr, nullptr, true));
    masterParams->setMasterSemitoneTunePtr (parameters.getRawParameterValue (("masterSemitoneTune")));
    parameters.addParameterListener ("masterSemitoneTune", this);

    // Master fine tuning
    parameters.createAndAddParameter (std::make_unique<Parameter> ("masterFineTune", "Fine Tune", "", nrange, 0.5, valueToMinusOneToOneFunction, nullptr, true));
    masterParams->setMasterFineTunePtr (parameters.getRawParameterValue (("masterFineTune")));
    parameters.addParameterListener ("masterFineTune", this);

    // Portamento
    parameters.createAndAddParameter (std::make_unique<Parameter> ("portamento", "Portamento", "", nrange, 0.0, valueToTextFunction, nullptr, true));
    masterParams->setPortamentoPtr (parameters.getRawParameterValue (("portamento")));
    parameters.addParameterListener ("portamento", this);

    // Master volume
    parameters.createAndAddParameter (std::make_unique<Parameter> ("masterVolume", "Master Vol", "", nrange, 0.5, valueToTextFunction, nullptr, true));
    masterParams->setMasterVolumePtr (parameters.getRawParameterValue (("masterVolume")));
    parameters.addParameterListener ("masterVolume", this);

    // ---

    // Parameters used with callback

    // Number of voices
    // TODO: We need a smarter way to set the initial value.
    constexpr float defaultFlnumNumVoices = 0.285; // The number will be converted to 8. OS-251 has 8 voices as default.
    parameters.createAndAddParameter (std::make_unique<Parameter> ("numVoices", "Num Voices", "", nrange, defaultFlnumNumVoices, numVoicesToStr, nullptr, true));
    parameters.addParameterListener ("numVoices", this);

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
    synthEngine.prepareToPlay (samplesPerBlock, sampleRate);
    synthParams.prepareToPlay (samplesPerBlock, sampleRate);
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
    // Host inforrmation
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

    synthEngine.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
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
    return new Os251AudioProcessorEditor (*this, parameters, presetManager);
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
            parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
            presetManager.requireToUpdatePresetNameOnUI();
        }
}

void Os251AudioProcessor::parameterChanged (const juce::String& parameterID, float newValue)
{
    // TODO: Update parameters in an efficient way
    onsen::OscillatorParams* const oscillatorParams = synthParams.oscillator();
    onsen::EnvelopeParams* const envelopeParams = synthParams.envelope();
    onsen::LfoParams* const lfoParams = synthParams.lfo();
    onsen::FilterParams* const filterParams = synthParams.filter();
    onsen::ChorusParams* const chorusParams = synthParams.chorus();
    onsen::HpfParams* const hpfParams = synthParams.hpf();
    onsen::MasterParams* const master = synthParams.master();
    oscillatorParams->parameterChanged();
    envelopeParams->parameterChanged();
    lfoParams->parameterChanged();
    filterParams->parameterChanged();
    chorusParams->parameterChanged();
    hpfParams->parameterChanged();
    master->parameterChanged();

    if (parameterID == "numVoices")
    {
        const int num = onsen::DspUtil::mapFlnumToInt (newValue, 0.0, 1.0, 1, onsen::MasterParams::maxNumVoices);
        synthEngine.changeNumberOfVoices (num);
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new Os251AudioProcessor();
}
