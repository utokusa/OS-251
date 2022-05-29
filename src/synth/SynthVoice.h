/*
  ==============================================================================

   OS-251 synthesizer's voice

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "../dsp/Filter.h"
#include "../dsp/IAudioBuffer.h"
#include "../dsp/Oscillator.h"
#include "SynthParams.h"
#include <memory>

namespace onsen
{
//==============================================================================
class ISynthVoice
{
    using flnum = float;

public:
    virtual ~ISynthVoice() = default;
    virtual void setCurrentPlaybackSampleRate (const double newRate) = 0;
    virtual void startNote (int midiNoteNumber, flnum velocity, int currentPitchWheelPosition) = 0;
    virtual void stopNote (flnum /*velocity*/, bool allowTailOff) = 0;
    virtual void setPitchWheel (int newPitchWheelValue) = 0;
    virtual void renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples) = 0;
    virtual void addPhaseOffset (flnum offset) = 0;
    virtual void setDetune (flnum val) = 0;
};
//==============================================================================
class FancySynthVoice : public ISynthVoice
{
    using flnum = float;

public:
    FancySynthVoice() = delete;
    FancySynthVoice (SynthParams* const synthParams, Lfo* const _lfo)
        : sampleRate (DEFAULT_SAMPLES_PER_BLOCK),
          p (synthParams->master()),
          smoothedAngleDelta (0.0, 0.0),
          smoothedAmp (0.0, 0.995),
          osc (synthParams->oscillator()),
          env ((IEnvelopeParams*) (synthParams->envelope())),
          gate(),
          envManager (&env, &gate),
          lfo (_lfo),
          filter (synthParams->filter(), &env, lfo),
          isNoteOn (false),
          isNoteOverlapped (false),
          detune (0.0)
    {
    }

    static std::vector<std::shared_ptr<ISynthVoice>> buildVoices (int maxNumVoices, SynthParams* const synthParams, Lfo* const _lfo)
    {
        std::vector<std::shared_ptr<ISynthVoice>> voices (maxNumVoices);
        for (int i = 0; i < maxNumVoices; i++)
        {
            voices[i] = std::make_shared<FancySynthVoice> (synthParams, _lfo);
        }
        return voices;
    };

    void setCurrentPlaybackSampleRate (const double newRate) override;
    void startNote (int midiNoteNumber, flnum velocity, int currentPitchWheelPosition) override;
    void stopNote (flnum /*velocity*/, bool allowTailOff) override;
    void setPitchWheel (int newPitchWheelValue) override;
    void renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples) override;

    void addPhaseOffset (flnum offset) override
    {
        assert (0.0 <= offset && offset <= pi * 2.0 + EPSILON);
        currentAngle += offset;
        if (currentAngle > pi * 2.0)
        {
            currentAngle -= pi * 2.0;
        }
    }

    void setDetune (flnum val) override
    {
        detune = val;
    }

private:
    double sampleRate;
    MasterParams* const p;
    // We use angle in radian
    flnum currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    SmoothFlnum smoothedAngleDelta;
    SmoothFlnum smoothedAmp;
    flnum pitchBend = 1.0;
    Oscillator osc;
    Envelope env;
    Gate gate;
    EnvManager envManager;
    Lfo* const lfo;
    Filter filter;
    bool isNoteOn;
    bool isNoteOverlapped;
    flnum detune;

    void setPitchBend (int pitchWheelValue);
    flnum midiNoteToHertz (int midiNote);
    bool isVoiceOff();
};
} // namespace onsen
