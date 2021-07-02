/*
  ==============================================================================

   Chorus Test

  ==============================================================================
*/

#include "../src/Chorus.h"
#include "../src/AudioBufferMock.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// Chorus

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

class ChorusTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        chorus.setCurrentPlaybackSampleRate (sampleRate);
        setTestInput1 (&audioBuffer);
    }

    // void TearDown() override {}

    static constexpr double sampleRate = 48000.0;
    static constexpr int numChannel = 2;
    static constexpr int samplesPerBlock = 512;

    Chorus chorus;
    AudioBufferMock audioBuffer { numChannel, samplesPerBlock };
};

TEST_F (ChorusTest, Snapshot)
{
    chorus.render (&audioBuffer, 0, samplesPerBlock - 1);
    // Some random snapshot
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 20), 0.3125);
    // This chorus is mono, so every channel should have same vaule.
    EXPECT_FLOAT_EQ (audioBuffer.getSample (1, 20), 0.3125);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 77), 0.203125);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, samplesPerBlock - 1), 0.984375);
}

} // namespace onsen
