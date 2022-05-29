/*
  ==============================================================================

   SynthParams Mock

  ==============================================================================
*/

#include "../../src/synth/SynthParams.h"
#include <atomic>
#include <memory>

namespace onsen
{
struct SynthParamsMockValues
{
public:
    using flnum = float;

    std::unique_ptr<SynthParams> buildSynthParams()
    {
        auto synthParams = std::make_unique<SynthParams>();
        // Oscillator parameters
        onsen::OscillatorParams* const oscillatorParams = synthParams->oscillator();
        oscillatorParams->setSinGainPtr (&sinGain);
        oscillatorParams->setSquareGainPtr (&squareGain);
        oscillatorParams->setSawGainPtr (&sawGain);
        oscillatorParams->setSubSquareGainPtr (&subSquareGain);
        oscillatorParams->setNoiseGainPtr (&noiseGain);
        oscillatorParams->setShapePtr (&shape);

        // Envelop parameters
        onsen::EnvelopeParams* const envelopeParams = synthParams->envelope();
        envelopeParams->setAttackPtr (&attack);
        envelopeParams->setDecayPtr (&decay);
        envelopeParams->setSustainPtr (&sustain);
        envelopeParams->setReleasePtr (&release);

        // LFO parameters
        onsen::LfoParams* const lfoParams = synthParams->lfo();
        lfoParams->setRatePtr (&rate);
        lfoParams->setRateSyncPtr (&rate);
        lfoParams->setPhasePtr (&lfoPhase);
        lfoParams->setDelayPtr (&delay);
        lfoParams->setSyncOnPtr (&syncOn);
        lfoParams->setPitchPtr (&pitch);
        lfoParams->setFilterFreqPtr (&filterFreq);
        lfoParams->setShapePtr (&lfoShape);

        // Filter parameters
        onsen::FilterParams* const filterParams = synthParams->filter();
        filterParams->setFrequencyPtr (&frequency);
        filterParams->setResonancePtr (&resonance);
        filterParams->setFilterEnvelopePtr (&filterEnvelope);

        // HPF parameters
        onsen::HpfParams* const hpfParams = synthParams->hpf();
        hpfParams->setFrequencyPtr (&hpfFreq);

        // Chorus parameters
        onsen::ChorusParams* const chorusParams = synthParams->chorus();
        chorusParams->setChorusOnPtr (&chorusOn);

        // Master parameters
        onsen::MasterParams* const masterParams = synthParams->master();
        masterParams->setEnvForAmpOnPtr (&envForAmpOn);
        masterParams->setPitchBendWidthPtr (&pitchBendWidth);
        masterParams->setMasterOctaveTunePtr (&masterOctaveTune);
        masterParams->setMasterSemitoneTunePtr (&masterSemitoneTune);
        masterParams->setMasterFineTunePtr (&masterFineTune);
        masterParams->setPortamentoPtr (&portamento);
        masterParams->setMasterVolumePtr (&masterVolume);
        return std::move (synthParams);
    }

    std::atomic<flnum> sinGain = { 0.5f };
    std::atomic<flnum> squareGain = { 0.5f };
    std::atomic<flnum> sawGain = { 0.5f };
    std::atomic<flnum> subSquareGain = { 0.5f };
    std::atomic<flnum> noiseGain = { 0.5f };
    std::atomic<flnum> shape = { 0.5f };

    std::atomic<flnum> attack = { 0.5f };
    std::atomic<flnum> decay = { 0.5f };
    std::atomic<flnum> sustain = { 0.5f };
    std::atomic<flnum> release = { 0.5f };

    std::atomic<flnum> rate = { 0.5f }; // LFO rate
    std::atomic<flnum> rateSync = { 0.5f }; // LFO synced rate
    std::atomic<flnum> lfoPhase = { 0.5f }; // LFO delay
    std::atomic<flnum> delay = { 0.5f }; // LFO delay
    std::atomic<flnum> syncOn = { 0.0f }; // LFO sync ON
    std::atomic<flnum> pitch = { 0.5f }; // Amount of modulation
    std::atomic<flnum> filterFreq = { 0.5f }; // Amount of modulation
    std::atomic<flnum> lfoShape = { 0.5f };

    std::atomic<flnum> frequency = { 0.5f };
    std::atomic<flnum> resonance = { 0.5f };
    std::atomic<flnum> filterEnvelope = { 0.5f };

    std::atomic<flnum> hpfFreq = { 0.0f };

    std::atomic<flnum> chorusOn = { 1.0f };

    std::atomic<flnum> envForAmpOn = { 1.0f };
    std::atomic<flnum> pitchBendWidth = { 0.5 };
    std::atomic<flnum> masterOctaveTune = { 0.5 };
    std::atomic<flnum> masterSemitoneTune = { 0.5 };
    std::atomic<flnum> masterFineTune = { 0.5 };
    std::atomic<flnum> portamento = { 0.0f };
    std::atomic<flnum> masterVolume = { 1.0f };
};
} // namespace onsen