/*
  ==============================================================================

   Chorus Test

  ==============================================================================
*/

#include "../../src/dsp/Chorus.h"
#include "util/AudioBufferMock.h"
#include "util/TestAudioBufferInput.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// Chorus

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
    static constexpr int samplesPerBlock = 2048; // Require more that the chorus' buffer size

    Chorus chorus;
    AudioBufferMock audioBuffer { numChannel, samplesPerBlock };
};

TEST_F (ChorusTest, Snapshot)
{
    chorus.render (&audioBuffer, 0, samplesPerBlock - 1);
    // Some random snapshot
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 20), 0.078125);
    // This chorus is mono, so every channel should have same value.
    EXPECT_FLOAT_EQ (audioBuffer.getSample (1, 20), 0.078125); // Before chorus' buffer is initialized.
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 77), 0.30078125); // Before chorus' buffer is initialized.
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 1000), 0.98160553);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, samplesPerBlock - 1), 0.99609375);
}

} // namespace onsen
