/*
  ==============================================================================

    JuceSynthEngineAdapter

  ==============================================================================
*/

#pragma once

#include "../synth/SynthEngine.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class JuceSynthEngineAdapter
{
public:
    JuceSynthEngineAdapter() = delete;
    JuceSynthEngineAdapter (SynthParams* const synthParams, IPositionInfo* const positionInfo)
        : lfo (synthParams->lfo(), positionInfo),
          voices (FancySynthVoice::buildVoices (SynthEngine::getMaxNumVoices(), synthParams, &lfo)),
          synth (synthParams, positionInfo, &lfo, voices)
    {
    }

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate)
    {
        synth.setCurrentPlaybackSampleRate (sampleRate);
        synth.setSamplesPerBlock (samplesPerBlockExpected);
    }

    void releaseResources() {}

    void renderNextBlock (IAudioBuffer* outputAudio, const juce::MidiBuffer& inputMidi, int startSample, int numSamples)
    {
        if (! inputMidi.getNumEvents())
        {
            synth.renderNextBlock (outputAudio, startSample, numSamples);
            return;
        }
        int firstEventTime = inputMidi.getFirstEventTime();
        synth.renderNextBlock (outputAudio, startSample, std::max (0, firstEventTime - startSample));
        auto it = inputMidi.begin();
        while (it != inputMidi.end())
        {
            auto metadata = *it;
            int curSample = metadata.samplePosition;
            while (it != inputMidi.end() && curSample == (*it).samplePosition)
            {
                // consume event
                auto midiMsg = (*it).getMessage();
                int midiStatusByte = (int) (*(midiMsg.getRawData()));
                if (midiMsg.isNoteOff())
                {
                    synth.noteOff (midiMsg.getNoteNumber());
                }
                else if (midiMsg.isNoteOn())
                {
                    synth.noteOn (midiMsg.getNoteNumber(), static_cast<int> (midiMsg.getVelocity()));
                }
                else if (midiMsg.isAllNotesOff())
                {
                    synth.allNoteOff();
                }
                else if (midiMsg.isPitchWheel())
                {
                    synth.updatePitchWheel (midiMsg.getPitchWheelValue());
                }
                it++;
            }
            // render audio
            int numSamplesOfSection = it == inputMidi.end() ? startSample + numSamples - curSample : (*it).samplePosition - curSample;
            synth.renderNextBlock (outputAudio, curSample, numSamplesOfSection);
        }
    }

    void changeNumberOfVoices (int num)
    {
        synth.setNumberOfVoices (num);
    }

private:
    Lfo lfo;
    std::vector<std::shared_ptr<ISynthVoice>> voices;
    SynthEngine synth;
};
} // namespace onsen
