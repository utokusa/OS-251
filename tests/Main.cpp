#include <JuceHeader.h>
#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST (HelloTest, BasicAssertions)
{
    // Expect two strings not to be equal.
    EXPECT_STRNE ("hello", "world");
    // Expect equality.
    EXPECT_EQ (7 * 6, 42);
}

TEST (HelloTest, juceString)
{
    juce::String hello("hello");
    // Expect two strings to be equal.
    EXPECT_STREQ (hello.toRawUTF8(), "hello");
}