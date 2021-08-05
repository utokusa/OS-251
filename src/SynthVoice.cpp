/*
  ==============================================================================

   OS-251 synthesizer's voice

  ==============================================================================
*/

#include "SynthVoice.h"

namespace onsen
{
//==============================================================================
bool FancySynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<FancySynthSound*> (sound) != nullptr;
}

void FancySynthVoice::setCurrentPlaybackSampleRate (const double newRate)
{
    juce::SynthesiserVoice::setCurrentPlaybackSampleRate (newRate);
    envManager.setCurrentPlaybackSampleRate (newRate);
    filter.setCurrentPlaybackSampleRate (newRate);
}

void FancySynthVoice::startNote (int midiNoteNumber, flnum velocity, juce::SynthesiserSound*, int currentPitchWheelPosition)
{
    setPitchBend (currentPitchWheelPosition);

    level = velocity; // The max value of velocity is 1.0
    envManager.noteOn();

    flnum adjustOctave = 2.0;
    flnum cyclesPerSecond = juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber) / adjustOctave;
    flnum cyclesPerSample = cyclesPerSecond / getSampleRate();

    angleDelta = cyclesPerSample * 2.0 * pi;
    if (isNoteOverlapped)
    {
        smoothedAngleDelta.set (angleDelta);
    }
    else
    {
        smoothedAngleDelta.reset (angleDelta);
    }

    lfo->noteOn();
    isNoteOn = true;
}

void FancySynthVoice::stopNote (float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        // Change state to RELEASE
        envManager.noteOff();
        isNoteOverlapped = false;
    }
    else
    {
        // Change note immediatelly
        clearCurrentNote();
        angleDelta = 0.0;
        isNoteOverlapped = isNoteOn;
    }

    lfo->noteOff();
    isNoteOn = false;
}

void FancySynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    setPitchBend (newPitchWheelValue);
}

void FancySynthVoice::renderNextBlock (juce::AudioSampleBuffer& outputBuffer, int startSample, int numSamples)
{
    int idx = startSample;
    if (angleDelta != 0.0)
    {
        while (--numSamples >= 0)
        {
            envManager.switchTarget (p->getEnvForAmpOn());
            flnum currentSample = osc.oscillatorVal (
                currentAngle, lfo->getLevel (idx) * lfo->getShapeAmount());
            flnum rawAmp = level * envManager.getLevel();
            smoothedAmp.set (rawAmp);
            smoothedAmp.update();
            currentSample = filter.process (currentSample, idx);
            currentSample *= smoothedAmp.get();

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, idx, currentSample);

            flnum lfoPitchDepth = lfo->getPitchAmount();
            flnum lfoVal = lfo->getLevel (idx);
            smoothedAngleDelta.setSmoothness (p->getPortamento());
            smoothedAngleDelta.update();
            currentAngle += smoothedAngleDelta.get() * p->getFreqRatio() * (1.0 * pitchBend + lfoPitchDepth * lfoVal);
            if (currentAngle > pi * 2.0)
            {
                currentAngle -= pi * 2.0;
            }
            ++idx;
            envManager.update();
            smoothedAmp.update();
            if (envManager.isEnvOff() && smoothedAmp.get() <= 0.001)
            {
                smoothedAmp.reset (0.0);
                angleDelta = 0.0;
                smoothedAngleDelta.reset (angleDelta);
                clearCurrentNote();
                break;
            }
        }
    }
}

//==============================================================================
void FancySynthVoice::setPitchBend (int pitchWheelValue)
{
    // `newPitchWheelValue` is integer from 0 to 16383 (0x3fff).
    // 8192 -> no pitch bend
    if (pitchWheelValue > 8192)
    {
        pitchBend = 1.0 + (p->getPitchBendWidthInFreqRatio() - 1.0) * (static_cast<flnum> (pitchWheelValue) - 8192.0) / 8191.0; // 16383 - 8192 = 8191
    }
    else if (pitchWheelValue == 8192)
    {
        pitchBend = 1.0;
    }
    else
    {
        pitchBend = 1.0 / (1.0 + (p->getPitchBendWidthInFreqRatio() - 1.0) * (8192.0 - static_cast<flnum> (pitchWheelValue)) / 8192.0);
    }
}
} // namespace onsen
