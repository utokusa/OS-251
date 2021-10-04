/*
  ==============================================================================
   Tmp File Manager Test
  ==============================================================================
*/

#include "../../src/services/TmpFileManager.h"
#include <JuceHeader.h>
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
class TmpFileManagerTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Clean
        testTmpDir.deleteRecursively();
    }

    void TearDown() override
    {
        // Clean up test tmp dir
        testTmpDir.deleteRecursively();
    }

    const juce::File testTmpDir { onsen::TmpFileManager::getTmpDir().getChildFile ("tmp_file_test") };
};
//==============================================================================

TEST_F (TmpFileManagerTest, GetTmpFileDir)
{
    // We have following assumption.
    // This test is not to miss the change of the specification on juce::File::getSpecialLocation()

    // juce::File::getSpecialLocation (juce::File::tempDirectory) returns:
    // - ~/Library/Caches/OS-251 for macOS
    // - C:¥Users¥<UserName>¥AppData¥Local¥Temp for windows
    // - /tmp for Linux
#if JUCE_MAC
    EXPECT_EQ (TmpFileManager::getTmpDir(), juce::File ("~/Library/Caches/Onsen Audio/OS-251"));
#elif JUCE_WINDOWS
    // TODO: Maybe we can improve the test
    EXPECT_EQ (TmpFileManager::getTmpDir(),
               juce::File::getSpecialLocation (juce::File::tempDirectory).getChildFile ("Onsen Audio/OS-251"));
#elif JUCE_LINUX
    EXPECT_EQ (TmpFileManager::getTmpDir(), juce::File ("/tmp/Onsen Audio/OS-251"));
#endif
}

TEST_F (TmpFileManagerTest, CreateTmpFile)
{
    auto tmpFile = TmpFileManager::createTempFile (testTmpDir, "main.js");
    EXPECT_TRUE (tmpFile.isAChildOf (testTmpDir));
    // The file name should be something like "temp_6e09bcc5.main.js".
    EXPECT_TRUE (tmpFile.getFileName().startsWith ("temp_"));
    EXPECT_TRUE (tmpFile.getFileName().endsWith (".main.js"));
}

TEST_F (TmpFileManagerTest, CleanUpTmpDirToDeleteFilesMoreThanOneDayOld)
{
    testTmpDir.createDirectory();
    auto file1 = testTmpDir.getChildFile ("temp_01.main.js");
    auto file2 = testTmpDir.getChildFile ("temp_02.main.js");
    auto file3 = testTmpDir.getChildFile ("temp_03.main.js");

    file1.create();
    file2.create();
    file3.create();

    auto current = juce::Time::getCurrentTime();
    auto file1Created = current;
    auto file2Created = current;
    file1Created -= juce::RelativeTime::days (1.5);
    file2Created -= juce::RelativeTime::days (0.5);
    bool ok1 = file1.setCreationTime (file1Created);
    bool ok2 = file2.setCreationTime (file2Created);

    EXPECT_EQ (testTmpDir.getNumberOfChildFiles (juce::File::TypesOfFileToFind::findFilesAndDirectories), 3);
    TmpFileManager::cleanUpTmpDir (testTmpDir);

    // POSIX based systems can't change create date.
    // https://forum.juce.com/t/modify-the-file-the-date-of-creation/43265/5
    // So for Unix, ok1 and ok2 are always false. This test is only useful for windows.
    if (ok1 && ok2)
    {
        EXPECT_EQ (testTmpDir.getNumberOfChildFiles (juce::File::TypesOfFileToFind::findFilesAndDirectories), 2);
    }
    else
    {
        EXPECT_EQ (testTmpDir.getNumberOfChildFiles (juce::File::TypesOfFileToFind::findFilesAndDirectories), 3);
    }
}

} // namespace onsen
