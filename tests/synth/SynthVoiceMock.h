/*
  ==============================================================================

   Mock SynthVoice

  ==============================================================================
*/

#pragma once

#include "../../src/synth/SynthVoice.h"
#include <iomanip>
#include <ios>
#include <sstream>

namespace onsen
{
//==============================================================================
class SynthVoiceMock : public ISynthVoice
{
    using flnum = float;

public:
    SynthVoiceMock() = delete;
    SynthVoiceMock (int voiceId, std::vector<std::string>& logs) : voiceId (voiceId), logs (logs) {}

    void setCurrentPlaybackSampleRate (const double newRate) override {};
    void startNote (int midiNoteNumber, flnum velocity, int currentPitchWheelPosition) override
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision (2);
        ss << "startNote: " << voiceId << " " << midiNoteNumber << " " << velocity << " " << currentPitchWheelPosition;
        logs.push_back (ss.str());
    }
    void stopNote (flnum /*velocity*/, bool allowTailOff) override
    {
        std::stringstream ss;
        ss << "stopNote: " << voiceId << " " << allowTailOff;
        logs.push_back (ss.str());
    }
    void pitchWheelMoved (int newPitchWheelValue) override
    {
        std::stringstream ss;
        ss << "pitchWheelMoved: " << voiceId << " " << newPitchWheelValue;
        logs.push_back (ss.str());
    }
    void controllerMoved (int, int) override {}
    void renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples) override {}

private:
    int voiceId;
    std::vector<std::string>& logs;
};
} // namespace onsen
