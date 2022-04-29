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

namespace onsen
{
//==============================================================================
class FancySynthVoice
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
          isNoteOverlapped (false)
    {
    }

    void setCurrentPlaybackSampleRate (const double newRate);
    void startNote (int midiNoteNumber, flnum velocity, int currentPitchWheelPosition);
    void stopNote (float /*velocity*/, bool allowTailOff);
    void pitchWheelMoved (int newPitchWheelValue);
    void controllerMoved (int, int) {}
    void renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples);

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

    void setPitchBend (int pitchWheelValue);
    flnum midiNoteToHertz (int midiNote);
};
} // namespace onsen
