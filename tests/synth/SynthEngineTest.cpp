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
    std::vector<std::shared_ptr<ISynthVoice>> voices { SynthVoiceMock::buildVoices (SynthEngine::getMaxNumVoices(), logs) };
    SynthEngine synth { synthParams.get(), &positionInfo, &lfo, voices };
};

TEST_F (SynthEngineTest, ConfirmMockVoicesAreCreatedCorrectly)
{
    for (int i = 0; i < SynthEngine::getMaxNumVoices(); i++)
    {
        ASSERT_EQ (std::dynamic_pointer_cast<SynthVoiceMock> (voices[i])->getVoiceId(), i);
    }
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceSimple1)
{
    synth.setNumberOfVoices (1);
    synth.noteOn (69, 100);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceSimple2)
{
    synth.setNumberOfVoices (1);
    synth.noteOn (69, 100);
    synth.noteOn (69, 100);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 0");
    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceSimple3)
{
    synth.setNumberOfVoices (1);
    synth.noteOn (69, 100);
    synth.noteOn (64, 100);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 0");
    ASSERT_EQ (logs[2], "startNote: 0 64 0.79 8192");
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceSimple4NoteOffWithZeroVelocity)
{
    synth.setNumberOfVoices (1);
    synth.noteOn (69, 100);
    synth.noteOn (69, 0 /*velocity is zero*/); // Should be treated as noteOff
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceCornerCase1)
{
    synth.setNumberOfVoices (1);
    synth.noteOn (69, 100);
    synth.noteOff (68); // To be ignored
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToOneVoiceSimpleWithPitchbend)
{
    synth.setNumberOfVoices (1);
    synth.setPitchWheel (10000);
    synth.noteOn (69, 127);
    synth.setPitchWheel (10001);
    synth.noteOn (64, 127);
    ASSERT_EQ (logs.size(), 5);
    ASSERT_EQ (logs[0], "setPitchWheel: 0 10000");
    ASSERT_EQ (logs[1], "startNote: 0 69 1.00 10000");
    ASSERT_EQ (logs[2], "setPitchWheel: 0 10001");
    ASSERT_EQ (logs[3], "stopNote: 0 0");
    ASSERT_EQ (logs[4], "startNote: 0 64 1.00 10001");
}

TEST_F (SynthEngineTest, SynthAssignNotesToTwoVoiceSimple1)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToTwoVoiceSimple2)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.noteOn (69, 100);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 0");
    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
}

TEST_F (SynthEngineTest, SynthAssignNotesToTwoVoiceSimple3)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.noteOn (64, 100);
    synth.noteOff (64);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 4);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "startNote: 1 64 0.79 8192");
    ASSERT_EQ (logs[2], "stopNote: 1 1");
    ASSERT_EQ (logs[3], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToTwoVoiceCornerCase1)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.noteOff (68); // To be ignored
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SynthAssignNotesToTwoVoiceSimpleWithPitchbend)
{
    synth.setNumberOfVoices (2);
    synth.setPitchWheel (10000);
    synth.noteOn (69, 127);
    synth.setPitchWheel (10001);
    synth.noteOn (64, 127);
    ASSERT_EQ (logs.size(), 6);
    ASSERT_EQ (logs[0], "setPitchWheel: 0 10000");
    ASSERT_EQ (logs[1], "setPitchWheel: 1 10000");
    ASSERT_EQ (logs[2], "startNote: 0 69 1.00 10000");
    ASSERT_EQ (logs[3], "setPitchWheel: 0 10001");
    ASSERT_EQ (logs[4], "setPitchWheel: 1 10001");
    ASSERT_EQ (logs[5], "startNote: 1 64 1.00 10001");
}

TEST_F (SynthEngineTest, SynthAssignNotesTo3VoiceComplex)
{
    synth.setNumberOfVoices (3);
    synth.noteOn (69, 100);
    synth.noteOn (70, 127);
    synth.noteOn (71, 50);
    synth.noteOff (69);
    synth.noteOn (72, 64);
    synth.noteOn (73, 1);
    ASSERT_EQ (logs.size(), 7);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "startNote: 1 70 1.00 8192");
    ASSERT_EQ (logs[2], "startNote: 2 71 0.39 8192");
    ASSERT_EQ (logs[3], "stopNote: 0 1");
    ASSERT_EQ (logs[4], "startNote: 0 72 0.50 8192");
    ASSERT_EQ (logs[5], "stopNote: 2 0");
    ASSERT_EQ (logs[6], "startNote: 2 73 0.01 8192");
}

TEST_F (SynthEngineTest, AllNotesOff)
{
    synth.setNumberOfVoices (3);
    synth.allNoteOff();
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, SetUnisonOn)
{
    synth.setNumberOfVoices (3);
    synth.setIsUnison (true);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, SetUnisonOff)
{
    synth.setNumberOfVoices (3);
    synth.setIsUnison (true);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, UnisonSimple1)
{
    synth.setNumberOfVoices (3);
    synth.setIsUnison (true);
    synth.noteOn (69, 100);
    synth.noteOff (69);

    ASSERT_EQ (logs.size(), 9);

    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");

    ASSERT_EQ (logs[3], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[4], "startNote: 1 69 0.79 8192");
    ASSERT_EQ (logs[5], "startNote: 2 69 0.79 8192");

    ASSERT_EQ (logs[6], "stopNote: 0 1");
    ASSERT_EQ (logs[7], "stopNote: 1 1");
    ASSERT_EQ (logs[8], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, UnisonSimple2)
{
    synth.setNumberOfVoices (2);
    synth.setIsUnison (true);

    synth.noteOn (69, 100);
    synth.noteOn (69, 100);
    ASSERT_EQ (logs.size(), 8);

    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");

    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[3], "startNote: 1 69 0.79 8192");

    ASSERT_EQ (logs[4], "stopNote: 0 0");
    ASSERT_EQ (logs[5], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[6], "stopNote: 1 0");
    ASSERT_EQ (logs[7], "startNote: 1 69 0.79 8192");
}

TEST_F (SynthEngineTest, UnisonSimple3)
{
    synth.setNumberOfVoices (2);
    synth.setIsUnison (true);

    synth.noteOn (69, 100);
    synth.noteOn (64, 100);
    ASSERT_EQ (logs.size(), 8);

    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");

    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[3], "startNote: 1 69 0.79 8192");

    ASSERT_EQ (logs[4], "stopNote: 0 0");
    ASSERT_EQ (logs[5], "startNote: 0 64 0.79 8192");
    ASSERT_EQ (logs[6], "stopNote: 1 0");
    ASSERT_EQ (logs[7], "startNote: 1 64 0.79 8192");
}

TEST_F (SynthEngineTest, UnisonSimple4NoteOffWithZeroVelocity)
{
    synth.setNumberOfVoices (3);
    synth.setIsUnison (true);
    synth.noteOn (69, 100);
    synth.noteOn (69, 0 /*velocity is zero*/); // Should be treated as noteOff

    ASSERT_EQ (logs.size(), 9);

    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");

    ASSERT_EQ (logs[3], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[4], "startNote: 1 69 0.79 8192");
    ASSERT_EQ (logs[5], "startNote: 2 69 0.79 8192");

    ASSERT_EQ (logs[6], "stopNote: 0 1");
    ASSERT_EQ (logs[7], "stopNote: 1 1");
    ASSERT_EQ (logs[8], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, UnisonCornerCase1)
{
    synth.setNumberOfVoices (3);
    synth.setIsUnison (true);
    synth.noteOn (69, 100);
    synth.noteOff (68); // To be ignored
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 9);

    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    ASSERT_EQ (logs[2], "stopNote: 2 1");

    ASSERT_EQ (logs[3], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[4], "startNote: 1 69 0.79 8192");
    ASSERT_EQ (logs[5], "startNote: 2 69 0.79 8192");

    ASSERT_EQ (logs[6], "stopNote: 0 1");
    ASSERT_EQ (logs[7], "stopNote: 1 1");
    ASSERT_EQ (logs[8], "stopNote: 2 1");
}

TEST_F (SynthEngineTest, SustainPedal1)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.setSustainPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 1);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    synth.setSustainPedalDown (false);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SustainPedal2)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.setSustainPedalDown (true);
    synth.noteOn (60, 100);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[1], "startNote: 1 60 0.79 8192");
    synth.setSustainPedalDown (false);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[2], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SustainPedalWithUnison)
{
    synth.setNumberOfVoices (2);
    synth.setIsUnison (true);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    synth.noteOn (69, 100);
    synth.setSustainPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 4);
    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[3], "startNote: 1 69 0.79 8192");
    synth.setSustainPedalDown (false);
    ASSERT_EQ (logs.size(), 6);
    ASSERT_EQ (logs[4], "stopNote: 0 1");
    ASSERT_EQ (logs[5], "stopNote: 1 1");
}

TEST_F (SynthEngineTest, SostenutoPedal)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.setSostenutoPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 1);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    synth.noteOn (60, 100);
    synth.noteOff (60);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[1], "startNote: 1 60 0.79 8192");
    ASSERT_EQ (logs[2], "stopNote: 1 1");
    synth.setSostenutoPedalDown (false);
    ASSERT_EQ (logs.size(), 4);
    ASSERT_EQ (logs[3], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SostenutoPedalWithUnison)
{
    synth.setNumberOfVoices (2);
    synth.setIsUnison (true);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[0], "stopNote: 0 1");
    ASSERT_EQ (logs[1], "stopNote: 1 1");
    synth.noteOn (69, 100);
    synth.setSostenutoPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 4);
    ASSERT_EQ (logs[2], "startNote: 0 69 0.79 8192");
    ASSERT_EQ (logs[3], "startNote: 1 69 0.79 8192");
    synth.setSostenutoPedalDown (false);
    ASSERT_EQ (logs.size(), 6);
    ASSERT_EQ (logs[4], "stopNote: 0 1");
    ASSERT_EQ (logs[5], "stopNote: 1 1");
}

TEST_F (SynthEngineTest, SustainAndSostenutoPedalAtTheSameTime1)
{
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.setSostenutoPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 1);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    synth.setSustainPedalDown (true); // It should not affect sostenuto pedal's behavior
    synth.setSustainPedalDown (false);
    ASSERT_EQ (logs.size(), 1);
    synth.setSostenutoPedalDown (false);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[1], "stopNote: 0 1");
}

TEST_F (SynthEngineTest, SustainAndSostenutoPedalAtTheSameTime2)
{
    // Similar to SustainAndSostenutoPedalAtTheSameTime1, but note 60 is added here
    synth.setNumberOfVoices (2);
    synth.noteOn (69, 100);
    synth.setSostenutoPedalDown (true);
    synth.noteOff (69);
    ASSERT_EQ (logs.size(), 1);
    ASSERT_EQ (logs[0], "startNote: 0 69 0.79 8192");
    synth.setSustainPedalDown (true); // It should not affect sostenuto pedal's behavior
    synth.noteOn (60, 100);
    synth.noteOff (60);
    ASSERT_EQ (logs.size(), 2);
    ASSERT_EQ (logs[1], "startNote: 1 60 0.79 8192");
    synth.setSustainPedalDown (false);
    ASSERT_EQ (logs.size(), 3);
    ASSERT_EQ (logs[2], "stopNote: 1 1");
    synth.setSostenutoPedalDown (false);
    ASSERT_EQ (logs.size(), 4);
    ASSERT_EQ (logs[3], "stopNote: 0 1");
}

// Assertion tests
// They don't pass on GitHub Actions. So I skip them for now.
// This PR might be related https://github.com/google/googletest/issues/234 .

// TEST_F (SynthEngineTest, WrongNumVoices)
// {
//     ASSERT_DEATH ({ synth.setNumberOfVoices (0); }, "1 <= num && num <= getMaxNumVoices()");
//     ASSERT_DEATH ({ synth.setNumberOfVoices (SynthEngine::getMaxNumVoices() + 1); }, "1 <= num && num <= getMaxNumVoices()");
// }

// TEST_F (SynthEngineTest, WrongVelocity)
// {
//     ASSERT_DEATH ({ synth.noteOn (69, -1); }, "0 <= intVelocity && intVelocity <= 127");
//     ASSERT_DEATH ({ synth.noteOn (69, 128); }, "0 <= intVelocity && intVelocity <= 127");
// }

// TEST_F (SynthEngineTest, WrongNoteNumber)
// {
//     ASSERT_DEATH ({ synth.noteOn (-1, 100); }, "0 <= noteNumber && noteNumber <= 127");
//     ASSERT_DEATH ({ synth.noteOn (128, 100); }, "0 <= noteNumber && noteNumber <= 127");
//     ASSERT_DEATH ({ synth.noteOff (-1); }, "0 <= noteNumber && noteNumber <= 127");
//     ASSERT_DEATH ({ synth.noteOff (128); }, "0 <= noteNumber && noteNumber <= 127");
// }

// TEST_F (SynthEngineTest, WrongPitchBend)
// {
//     ASSERT_DEATH ({ synth.setPitchWheel (-1); }, "0 <= val && val <= 16383");
//     ASSERT_DEATH ({ synth.setPitchWheel (16384); }, "0 <= val && val <= 16383");
// }
} // namespace onsen
