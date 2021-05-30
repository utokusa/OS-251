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

    currentAngle = 0.0;
    level = velocity * 0.15;
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
        filter.resetBuffer();
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
            currentSample = filter.process (currentSample, idx);
            currentSample *= level * envManager.getLevel();

            for (auto i = outputBuffer.getNumChannels(); --i >= 0;)
                outputBuffer.addSample (i, idx, currentSample);

            flnum lfoPitchDepth = lfo->getPitchAmount();
            flnum lfoVal = lfo->getLevel (idx);
            smoothedAngleDelta.setSmoothness (p->getPortamento());
            currentAngle += smoothedAngleDelta.get() * p->getFreqRatio() * (1.0 * pitchBend + lfoPitchDepth * lfoVal);
            if (currentAngle > pi * 2.0)
            {
                currentAngle -= pi * 2.0;
            }
            ++idx;
            envManager.update();
            if (envManager.isEnvOff())
            {
                clearCurrentNote();
                angleDelta = 0.0;
                smoothedAngleDelta.reset (angleDelta);
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
