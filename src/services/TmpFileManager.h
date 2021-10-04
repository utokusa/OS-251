/*
  ==============================================================================

   TmpFileManager

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
namespace TmpFileManager
{
    inline juce::File getTmpDir()
    {
        // juce::File::getSpecialLocation (juce::File::tempDirectory) returns:
        // - ~/Library/Caches/OS-251 for macOS
        // - C:¥Users¥<UserName>¥AppData¥Local¥Temp for windows
        // - /tmp for Linux

#if JUCE_MAC
        // We don't want to use "~/Library/Caches/OS-251"
        auto systemTmpDir = juce::File { "~/Library/Caches" };
#else
        auto systemTmpDir = juce::File::getSpecialLocation (juce::File::tempDirectory);
#endif
        return systemTmpDir.getChildFile ("Onsen Audio/OS-251");
    }

    inline juce::File createTempFile (const juce::File& dir, juce::StringRef fileNameEnding)
    {
        // juce::File::createTempFile() always create a file
        // under `juce::File::getSpecialLocation (juce::File::tempDirectory)`.
        // But it's not suitable for us.
        // So we use only file name.
        auto suggestedTmpFile = dir.createTempFile (fileNameEnding);
        auto tmpFileName = suggestedTmpFile.getFileName();
        auto tmpFile = dir.getChildFile (tmpFileName);

        if (tmpFile.exists())
            return createTempFile (dir, fileNameEnding);

        return tmpFile;
    }

    inline void cleanUpTmpDir (const juce::File& tmpDir)
    {
        // Clean up the tmp folder
        //
        // We have a rule about how to use the tmp folder that we
        // should not keep using one tmp file for more than a day.
        // So we delete files which are more than a day old.

        // Currently OS-251 use the tmp folder only for the js bundle.
        // The file name is something like "temp_6e09bcc5.main.js".
        auto files = tmpDir.findChildFiles (juce::File::TypesOfFileToFind::findFiles, false, "*.js");
        for (auto f : files)
        {
            long long createdAt = f.getCreationTime().toMilliseconds();
            long long current = juce::Time::getCurrentTime().toMilliseconds();
            constexpr long long millisecondsInDay = 1000 * 60 * 60 * 24; // 86400000
            if (current > createdAt + millisecondsInDay)
            {
                f.deleteFile();
            }
        }
    }

} // namespace TmpFileManager

} // namespace onsen
