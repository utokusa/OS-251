#include "../src/Envelope.h"
#include "../src/params/EnvelopeParamsMock.h"

#include "../src/Oscillator.h"
#include "../src/params/OscillatorParamsMock.h"
// #include <JuceHeader.h>
#include <gtest/gtest.h>
#include <vector>

namespace onsen
{
//==============================================================================
// Sample test using JUCE

// TEST (JuceLib, UseJuceString)
// {
//     juce::String hello ("hello");
//     // Expect two strings to be equal.
//     EXPECT_STREQ (hello.toRawUTF8(), "hello");
// }

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
    Envelope env { (IEnvelopeParams*) (&envParams) };
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

//==============================================================================
// Oscillator

TEST (OscillatorTest, Sin)
{
    // Only sin oscillator is used
    OscillatorParamsMock params { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that sin's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0), 0.5, EPSILON);
}

TEST (OscillatorTest, Square)
{
    // Only square oscillator is used
    OscillatorParamsMock params { 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that square's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0), 1.0, EPSILON);
}

TEST (OscillatorTest, Saw)
{
    // Only saw oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that saw's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0), -0.5, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0), 0.5, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0), -0.83333331346511841, EPSILON);
}

TEST (OscillatorTest, SubSquare)
{
    // Only sub square oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    Oscillator osc (&params);

    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0), 0.0), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi), 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0), 0.0), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0), 1.0, EPSILON);
}

TEST (OscillatorTest, Noise)
{
    // Only noise oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 };
    Oscillator osc (&params);

    int n = 1000;
    std::vector<flnum> vals;
    flnum mean = 0.0;
    // Generate value and calculate mean
    for (int i = 0; i < n; ++i)
    {
        flnum val = osc.oscillatorVal (0.0, 0.0);
        EXPECT_LE (val, 1.0);
        EXPECT_GE (val, 0.0);
        vals.push_back (val);
        mean += val;
    }
    mean /= n;

    constexpr flnum loose_epsilon = 0.1;
    EXPECT_NEAR (mean, 0.5, loose_epsilon);

    // Calculate variance
    flnum variance = 0.0;
    for (auto val : vals)
    {
        variance += std::pow ((val - mean), 2.0);
    }
    variance /= n;

    EXPECT_LT (variance, 0.5);
    EXPECT_GT (variance, 0.0);

    // EXPECT_NEAR(osc.oscillatorVal(0.0, 0.0), 1.0, EPSILON);
}

TEST (OscillatorTest, ChangeShapeAndMixWaves)
{
    // Start with shape = 0
    OscillatorParamsMock params { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0 };
    Oscillator osc (&params);

    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0), 2.1993587017059326, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0), 2.5326919555664062, EPSILON);
    params.shape = 0.5;
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0), 1.6666688919067383, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0), 2.1997506618499756, EPSILON);
    params.shape = 1.0;
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0), 1.0000048875808716, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0), 1.0012624263763428, EPSILON);

}

} // namespace onsen
