/*
  ==============================================================================

   Sample test using JUCE (NOT USED)

   If you want to run this test, comment out related lines in CMakeLists.txt

  ==============================================================================
*/

#include <gtest/gtest.h>
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
TEST (JuceLib, UseJuceString)
{
    juce::String hello ("hello");
    EXPECT_STREQ (hello.toRawUTF8(), "hello");
}

} // namespace onsen
