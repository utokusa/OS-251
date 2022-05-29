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
    JuceSynthEngineAdapter (SynthEngine& synth)
        : synth (synth)
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
                    synth.setPitchWheel (midiMsg.getPitchWheelValue());
                }
                else if (midiMsg.isSustainPedalOn())
                {
                    synth.setSustainPedalDown (true);
                }
                else if (midiMsg.isSustainPedalOff())
                {
                    synth.setSustainPedalDown (false);
                }
                else if (midiMsg.isSostenutoPedalOn())
                {
                    synth.setSostenutoPedalDown (true);
                }
                else if (midiMsg.isSostenutoPedalOff())
                {
                    synth.setSostenutoPedalDown (false);
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

    void changeIsUnison (int val)
    {
        synth.setIsUnison (val);
    }

private:
    SynthEngine& synth;
};
} // namespace onsen
