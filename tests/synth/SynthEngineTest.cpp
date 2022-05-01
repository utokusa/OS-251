/*
  ==============================================================================

   SynthEngine Test

  ==============================================================================
*/

#include "../../src/synth/SynthEngine.h"
#include "../dsp/util/PositionInfoMock.h"
#include "SynthParamsMock.h"
#include "SynthVoiceMock.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// SynthEngine

class SynthEngineTest : public ::testing::Test
{
protected:
    // void SetUp() override {}
    // void TearDown() override {}

    static constexpr double sampleRate = 44100;
    static constexpr flnum samplesPerBlock = 512;
    SynthParamsMockValues synthParamsMockValues {};
    std::unique_ptr<SynthParams> synthParams { synthParamsMockValues.buildSynthParams() };
    PositionInfoMock positionInfo {};
    Lfo lfo { synthParams->lfo(), &positionInfo };
    std::vector<std::string> logs;
    SynthVoiceMock voice { 0, logs };
    SynthEngine synth { synthParams.get(), &positionInfo, &lfo, &voice };
};

TEST_F (SynthEngineTest, SynthAssignNotesToVoiceSimple1)
{
    synth.noteOn (69, 100);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToVoiceSimple2)
{
    synth.noteOn (69, 100);
    synth.noteOn (69, 100);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 0");
    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
}

TEST_F (SynthEngineTest, SynthAssignNotesToVoiceSimple3)
{
    synth.noteOn (69, 100);
    synth.noteOn (64, 100);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 0");
    ASSERT_EQ (logs[2], "startNote: 0 64 0.79 8192");
}

TEST_F (SynthEngineTest, SynthAssignNotesToVoiceComplex1)
{
    synth.noteOn (69, 100);
    synth.noteOff (68); // To be ignored
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}
} // namespace onsen
