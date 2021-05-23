/*
  ==============================================================================

   OS-251 synthesizer's voice

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"
#include "Filter.h"
#include "Oscillator.h"
#include "SynthParams.h"
#include "SynthSound.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class FancySynthVoice : public juce::SynthesiserVoice
{
    using flnum = float;

public:
    FancySynthVoice() = delete;
    FancySynthVoice (SynthParams* const synthParams, Lfo* const _lfo)
        : p (synthParams->master()),
          smoothedAngleDelta (0.0, 0.0),
          osc (synthParams),
          env (synthParams),
          gate(),
          envManager (&env, &gate),
          lfo (_lfo),
          filter (synthParams, &env, lfo),
          isNoteOn (false),
          isNoteOverlapped (false)
    {
    }

    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void setCurrentPlaybackSampleRate (const double newRate) override;
    void startNote (int midiNoteNumber, flnum velocity, juce::SynthesiserSound*, int currentPitchWheelPosition) override;
    void stopNote (float /*velocity*/, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int, int) override {}
    void renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples) override;

private:
    MasterParams* const p;
    // We use angle in radian
    flnum currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    flnum currentPitchFlutterAngle = 0.0;
    SmoothFlnum smoothedAngleDelta;
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
};
} // namespace onsen
