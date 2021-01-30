/*
  ==============================================================================

   OS-251 synthesizer's voice

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DspCommon.h"
#include "SynthParams.h"
#include "SynthSound.h"
#include "Filter.h"
#include "Oscillator.h"

namespace onsen
{
//==============================================================================
class FancySynthVoice : public juce::SynthesiserVoice
{
    using flnum = float;

public:
    FancySynthVoice() = delete;
    FancySynthVoice (SynthParams* const synthParams, Lfo* const _lfo)
        : osc (synthParams),
          env (synthParams),
          lfo (_lfo),
          filter (synthParams, &env, lfo)
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
    void setPitchBend (int pitchWheelValue);
    // We use angle in radian
    flnum currentAngle = 0.0, angleDelta = 0.0, level = 0.0;
    flnum pitchBend = 1.0;
    flnum pitchBendMax = 2.0;
    Oscillator osc;
    Envelope env;
    Lfo* const lfo;
    Filter filter;
};
}
