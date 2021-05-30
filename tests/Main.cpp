#include "../src/Envelope.h"
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

TEST (Envelope, NoteOn)
{
    SynthParams synthParams;
    Envelope env (&synthParams);
    EXPECT_TRUE(env.isEnvOff());
    env.noteOn();
    EXPECT_FALSE(env.isEnvOff());
}

TEST (Envelope, NoteOff)
{
    SynthParams synthParams;
    Envelope env (&synthParams);
    env.setCurrentPlaybackSampleRate(1.0);
    EXPECT_TRUE(env.isEnvOff());
    env.noteOn();
    EXPECT_FALSE(env.isEnvOff());
    env.noteOff();
    env.update();
    EXPECT_TRUE(env.isEnvOff());
}
} // namespace onsen