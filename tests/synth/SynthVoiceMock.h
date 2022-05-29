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

    static std::vector<std::shared_ptr<ISynthVoice>> buildVoices (int maxNumVoices, std::vector<std::string>& logs)
    {
        std::vector<std::shared_ptr<ISynthVoice>> voices (maxNumVoices);
        for (int i = 0; i < maxNumVoices; i++)
        {
            voices[i] = std::make_shared<SynthVoiceMock> (0, logs);
            std::dynamic_pointer_cast<SynthVoiceMock> (voices[i])->setVoiceId (i);
        }
        return voices;
    };

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
    void setPitchWheel (int newPitchWheelValue) override
    {
        std::stringstream ss;
        ss << "setPitchWheel: " << voiceId << " " << newPitchWheelValue;
        logs.push_back (ss.str());
    }
    void renderNextBlock (IAudioBuffer* outputBuffer, int startSample, int numSamples) override {}
    void addPhaseOffset (flnum offset) override {}
    void setDetune (flnum val) override {}

    void setVoiceId (int id) { voiceId = id; }
    int getVoiceId() { return voiceId; }

private:
    int voiceId;
    std::vector<std::string>& logs;
};
} // namespace onsen
