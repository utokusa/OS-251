/*
  ==============================================================================

   Test audio buffer input

  ==============================================================================
*/

#include "TestAudioBufferInput.h"

namespace onsen
{
void setTestInput1 (IAudioBuffer* audioBuffer)
{
    for (int i = 0; i < audioBuffer->getNumChannels(); i++)
    {
        for (int j = 0; j < audioBuffer->getNumSamples(); j++)
        {
            // Some random input
            flnum val = static_cast<flnum> ((j * 8) % audioBuffer->getNumSamples())
                        / static_cast<flnum> (audioBuffer->getNumSamples());
            audioBuffer->setSample (i, j, val);
        }
    }
}

// Set all value to val
void setTestInput2Constant (IAudioBuffer* audioBuffer, flnum val)
{
    for (int i = 0; i < audioBuffer->getNumChannels(); i++)
    {
        for (int j = 0; j < audioBuffer->getNumSamples(); j++)
        {
            audioBuffer->setSample (i, j, val);
        }
    }
}
} // namespace onsen
