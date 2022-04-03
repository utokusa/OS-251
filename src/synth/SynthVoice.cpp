/*
  ==============================================================================

   OS-251 synthesizer's voice

  ==============================================================================
*/

#include "SynthVoice.h"

namespace onsen
{
//==============================================================================
void FancySynthVoice::setCurrentPlaybackSampleRate (const double newRate)
{
    sampleRate = newRate;
    envManager.setCurrentPlaybackSampleRate (newRate);
    filter.setCurrentPlaybackSampleRate (newRate);
    osc.setCurrentPlaybackSampleRate (newRate);
    smoothedAmp.prepareToPlay (newRate);
    smoothedAngleDelta.prepareToPlay (newRate);
}

void FancySynthVoice::startNote (int midiNoteNumber, flnum velocity, int currentPitchWheelPosition)
{
    setPitchBend (currentPitchWheelPosition);

    level = velocity; // The max value of velocity is 1.0
    envManager.noteOn();

    flnum adjustOctave = 2.0;
    flnum cyclesPerSecond = midiNoteToHertz (midiNoteNumber) / adjustOctave;
    flnum cyclesPerSample = cyclesPerSecond / sampleRate;

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
        // clearCurrentNote(); // It's to notify synth engine when it becomes poly synth
        angleDelta = 0.0;
        isNoteOverlapped = isNoteOn;
    }

    lfo->noteOff();
    isNoteOn = false;
}

void FancySynthVoice::setPitchWheel (int newPitchWheelValue)
{
    setPitchBend (newPitchWheelValue);
}

void FancySynthVoice::renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples)
{
    if (isVoiceOff())
        return;
    int idx = startSample;
    if (angleDelta != 0.0)
    {
        while (--numSamples >= 0)
        {
            envManager.switchTarget (p->getEnvForAmpOn());
            flnum currentSample = osc.oscillatorVal (
                currentAngle, lfo->getLevel (idx) * lfo->getShapeAmount(), smoothedAngleDelta.get());
            flnum rawAmp = level * envManager.getLevel();
            smoothedAmp.set (rawAmp);
            smoothedAmp.update();
            currentSample = filter.process (currentSample, idx);
            currentSample *= smoothedAmp.get();

            for (auto i = outputBuffer->getNumChannels(); --i >= 0;)
            {
                flnum* bufferPtr = outputBuffer->getWritePointer (i);
                bufferPtr[idx] += currentSample;
            }

            flnum lfoPitchDepth = lfo->getPitchAmount();
            flnum lfoVal = lfo->getLevel (idx);
            smoothedAngleDelta.setSmoothness (p->getPortamento());
            smoothedAngleDelta.update();
            currentAngle += smoothedAngleDelta.get() * p->getFreqRatio() * (1.0 * pitchBend + detune + lfoPitchDepth * lfoVal);
            if (currentAngle > pi * 2.0)
            {
                currentAngle -= pi * 2.0;
            }
            ++idx;
            envManager.update();
            smoothedAmp.update();
            if (isVoiceOff())
            {
                smoothedAmp.reset (0.0);
                angleDelta = 0.0;
                smoothedAngleDelta.reset (angleDelta);
                osc.resetState();
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

flnum FancySynthVoice::midiNoteToHertz (int midiNote)
{
    return 440.0 * std::pow (2.0, (midiNote - 69) / 12.0);
}

bool FancySynthVoice::isVoiceOff()
{
    return envManager.isEnvOff() && smoothedAmp.get() <= 0.001;
}
} // namespace onsen
