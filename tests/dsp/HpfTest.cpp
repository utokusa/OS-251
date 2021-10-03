/*
  ==============================================================================

   HPF Test

  ==============================================================================
*/

#include "../../src/dsp/Hpf.h"
#include "../../src/params/HpfParamsMock.h"
#include "util/AudioBufferMock.h"
#include "util/TestAudioBufferInput.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// HPF

class HpfTest : public ::testing::Test
{
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    static constexpr double sampleRate = 44100;
    static constexpr int samplesPerBlock = 512;
    HpfParamsMock hpfParam;
};

TEST_F (HpfTest, SnapshotFor1Ch)
{
    const int numChannels = 1;
    Hpf hpf { &hpfParam, numChannels };
    hpf.setCurrentPlaybackSampleRate (sampleRate);
    AudioBufferMock audioBuffer { numChannels, samplesPerBlock };
    setTestInput1 (&audioBuffer);
    hpf.render (&audioBuffer, 0, samplesPerBlock);

    // Snapshots
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 0), 0);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 20), 0.18569922);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, samplesPerBlock - 1), 0.2468688);
}

TEST_F (HpfTest, SnapshotFor2Ch)
{
    const int numChannels = 2;
    Hpf hpf { &hpfParam, numChannels };
    hpf.setCurrentPlaybackSampleRate (sampleRate);
    AudioBufferMock audioBuffer { numChannels, samplesPerBlock };
    setTestInput1 (&audioBuffer);
    hpf.render (&audioBuffer, 0, samplesPerBlock);

    // Snapshots
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 0), 0);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, 20), 0.18569922);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (0, samplesPerBlock - 1), 0.2468688);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (1, 0), 0);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (1, 20), 0.18569922);
    EXPECT_FLOAT_EQ (audioBuffer.getSample (1, samplesPerBlock - 1), 0.2468688);
}
} // namespace onsen
