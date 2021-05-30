#include "../src/Envelope.h"
#include "../src/params/EnvelopeParamsMock.h"
// #include <JuceHeader.h>
#include <gtest/gtest.h>

namespace onsen
{
// TEST (JuceLib, UseJuceString)
// {
//     juce::String hello ("hello");
//     // Expect two strings to be equal.
//     EXPECT_STREQ (hello.toRawUTF8(), "hello");
// }

void updateEnv (IEnvelope* env, int times)
{
    for (int i = 0; i < times; i++)
    {
        env->update();
    }
}

TEST (Envelope, NoteOn)
{
    EnvelopeParamsMock envParams;
    Envelope env ((IEnvelopeParams*) (&envParams));
    EXPECT_TRUE (env.isEnvOff());
    env.noteOn();
    EXPECT_FALSE (env.isEnvOff());
}

TEST (Envelope, NoteOff)
{
    EnvelopeParamsMock envParams;
    Envelope env ((IEnvelopeParams*) (&envParams));
    // 1 sec per one env.update()
    env.setCurrentPlaybackSampleRate (1.0);
    EXPECT_TRUE (env.isEnvOff());
    env.noteOn();
    EXPECT_FALSE (env.isEnvOff());
    env.noteOff();
    // The release of mock env is 3 sec
    updateEnv (&env, 3);
    EXPECT_TRUE (env.isEnvOff());
}

TEST (Envelope, NoteOffInSAttack)
{
    EnvelopeParamsMock envParams;
    Envelope env ((IEnvelopeParams*) (&envParams));
    // 1 sec per one env.update()
    env.setCurrentPlaybackSampleRate (1.0);
    EXPECT_TRUE (env.isEnvOff());
    EXPECT_FLOAT_EQ (env.getLevel(), 0.0);
    env.noteOn();
    // The attack of mock env is 3 sec
    updateEnv (&env, 1);
    EXPECT_LT (env.getLevel(), 1.0);
    env.noteOff();
    // The release of mock env is 3 sec
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}

TEST (Envelope, NoteOffInDecay)
{
    EnvelopeParamsMock envParams;
    Envelope env ((IEnvelopeParams*) (&envParams));
    // 1 sec per one env.update()
    env.setCurrentPlaybackSampleRate (1.0);
    EXPECT_TRUE (env.isEnvOff());
    EXPECT_FLOAT_EQ (env.getLevel(), 0.0);
    env.noteOn();
    // The attack of mock env is 3 sec
    updateEnv (&env, 3);
    EXPECT_FLOAT_EQ (env.getLevel(), 1.0);
    // The decay of mock env is 3 sec
    updateEnv (&env, 1);
    EXPECT_LT (0.5, env.getLevel());
    env.noteOff();
    // The release of mock env is 3 sec
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}

TEST (Envelope, NoteOffInSustain)
{
    EnvelopeParamsMock envParams;
    Envelope env ((IEnvelopeParams*) (&envParams));
    // 1 sec per one env.update()
    env.setCurrentPlaybackSampleRate (1.0);
    EXPECT_TRUE (env.isEnvOff());
    EXPECT_FLOAT_EQ (env.getLevel(), 0.0);
    env.noteOn();
    // The attack of mock env is 3 sec
    updateEnv (&env, 3);
    EXPECT_FLOAT_EQ (env.getLevel(), 1.0);
    // The decay of mock env is 3 sec
    updateEnv (&env, 3);
    // The mock sustain's value is 0.5
    EXPECT_FLOAT_EQ (env.getLevel(), 0.5f);
    // Sustain should keep its level
    updateEnv (&env, 100);
    EXPECT_FLOAT_EQ (env.getLevel(), 0.5f);
    env.noteOff();
    // The release of mock env is 3 sec
    updateEnv (&env, 2);
    EXPECT_FALSE (env.isEnvOff());
    updateEnv (&env, 1);
    EXPECT_TRUE (env.isEnvOff());
}
} // namespace onsen