/*
  ==============================================================================

   Envelope Test

  ==============================================================================
*/

#include "../../src/dsp/Envelope.h"
#include "../../src/params/EnvelopeParamsMock.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// Envelope

void updateEnv (IEnvelope* env, int times)
{
    for (int i = 0; i < times; i++)
    {
        env->update();
    }
}

class EnvelopeTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        env.setCurrentPlaybackSampleRate (sampleRate);
    }

    // void TearDown() override {}

    // 1 sec per one env.update()
    static constexpr double sampleRate = 1.0;
    EnvelopeParamsMock envParams;
    Envelope env { &envParams };
};

TEST_F (EnvelopeTest, NoteOn)
{
    EXPECT_TRUE (env.isEnvOff());
    env.noteOn();
    EXPECT_FALSE (env.isEnvOff());
}

TEST_F (EnvelopeTest, NoteOff)
{
    env.noteOn();
    env.noteOff();
    updateEnv (&env, 3);
    EXPECT_TRUE (env.isEnvOff());
}

TEST_F (EnvelopeTest, NoteOffInSAttack)
{
    env.noteOn();
    // Attack
    updateEnv (&env, 1);
    EXPECT_LT (env.getLevel(), 1.0);
    env.noteOff();
    // Release
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}

TEST_F (EnvelopeTest, NoteOffInDecay)
{
    EXPECT_FLOAT_EQ (env.getLevel(), 0.0);
    env.noteOn();
    // Attack
    updateEnv (&env, 3);
    EXPECT_FLOAT_EQ (env.getLevel(), 1.0);
    // Decay
    updateEnv (&env, 1);
    EXPECT_LT (0.5, env.getLevel());
    env.noteOff();
    // Release
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}

TEST_F (EnvelopeTest, NoteOffInSustain)
{
    EXPECT_FLOAT_EQ (env.getLevel(), 0.0);
    env.noteOn();
    // Attack
    updateEnv (&env, 3);
    EXPECT_FLOAT_EQ (env.getLevel(), 1.0);
    // Decay
    updateEnv (&env, 3);
    EXPECT_FLOAT_EQ (env.getLevel(), 0.5f);
    // Sustain
    updateEnv (&env, 100);
    EXPECT_FLOAT_EQ (env.getLevel(), 0.5f);
    env.noteOff();
    // Release
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}

//==============================================================================
// Gate

class GateTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        gate.setCurrentPlaybackSampleRate (sampleRate);
    }

    // void TearDown() override {}

    Gate gate;

    // NOTE: Following variables is available only for current implementation
    // Requires 2 samples to finish atack (When attack is 0.002 sec)
    static constexpr double sampleRate = 1000.0;
    static constexpr int requiredSampleFor2MiliSecAttack = 2;
    static constexpr int requiredSampleFor2MiliSecRelease = 2;
    static constexpr int marginSample = 1;
};

TEST_F (GateTest, NoteOn)
{
    EXPECT_TRUE (gate.isEnvOff());
    gate.noteOn();
    EXPECT_FALSE (gate.isEnvOff());
}

TEST_F (GateTest, NoteOff)
{
    gate.noteOn();
    gate.noteOff();
    updateEnv (&gate, requiredSampleFor2MiliSecRelease + marginSample);
    EXPECT_TRUE (gate.isEnvOff());
}

TEST_F (GateTest, NoteOffInSAttack)
{
    gate.noteOn();
    // Attack
    updateEnv (&gate, requiredSampleFor2MiliSecAttack - 1);
    EXPECT_LT (gate.getLevel(), 1.0);
    gate.noteOff();
    // Release
    updateEnv (&gate, requiredSampleFor2MiliSecRelease - 1);
    EXPECT_FALSE (gate.isEnvOff());
    updateEnv (&gate, requiredSampleFor2MiliSecRelease);
    EXPECT_TRUE (gate.isEnvOff());
}

TEST_F (GateTest, NoteOffInSustain)
{
    EXPECT_FLOAT_EQ (gate.getLevel(), 0.0);
    gate.noteOn();
    // Attack
    updateEnv (&gate, requiredSampleFor2MiliSecAttack + marginSample);
    EXPECT_FLOAT_EQ (gate.getLevel(), 1.0);
    // Sustain
    updateEnv (&gate, 100);
    EXPECT_FLOAT_EQ (gate.getLevel(), 1.0f);
    gate.noteOff();
    // Release
    EXPECT_FALSE (gate.isEnvOff());
    updateEnv (&gate, requiredSampleFor2MiliSecRelease + marginSample);
    EXPECT_TRUE (gate.isEnvOff());
}

//==============================================================================
// EnvManager

class EnvManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        envManager.setCurrentPlaybackSampleRate (sampleRate);
    }

    // void TearDown() override {}

    // 1 sec per one env.update()
    EnvelopeParamsMock envParams;
    Envelope env { (IEnvelopeParams*) (&envParams) };
    Gate gate;
    EnvManager envManager { &env, &gate };

    // NOTE: Following variables is available only for current implementation
    // Requires 2 samples to finish gate's atack (When attack is 0.002 sec)
    static constexpr double sampleRate = 1000.0;
    static constexpr int requiredSampleFor2MiliSecGateAttack = 2;
    static constexpr int requiredSampleFor2MiliSecGateRelease = 2;
    static constexpr int marginSample = 1;
};

TEST_F (EnvManagerTest, SwitchEnvelope)
{
    EXPECT_TRUE (envManager.isEnvOff());
    // Start note using env
    envManager.noteOn();
    EXPECT_FALSE (envManager.isEnvOff());
    // Finish gate attack
    updateEnv (&envManager, requiredSampleFor2MiliSecGateAttack + marginSample);
    // env is still less than the max amplitude 1.0
    EXPECT_LT (envManager.getLevel(), 1.0);
    // Switch to gate
    envManager.switchTarget (false);
    EXPECT_FLOAT_EQ (envManager.getLevel(), 1.0f);
    // Switch to env
    envManager.switchTarget (true);
    // env is still less than the max amplitude 1.0
    EXPECT_LT (envManager.getLevel(), 1.0);
}
} // namespace onsen
