/*
  ==============================================================================

   Logger

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>

namespace onsen
{
//==============================================================================
namespace Logger
{
    inline void write (std::string logMsg)
    {
#ifdef DEBUG
        static bool init = false;
        // std::string filePath = "log_OS-251.txt";
        juce::File file = juce::File::getSpecialLocation (
                              juce::File::SpecialLocationType::userDesktopDirectory)
                              .getChildFile ("log_OS-251.txt");
        std::string filePath = file.getFullPathName().toStdString();
        if (! init)
        {
            init = true;
            // Delete existing log
            std::remove (filePath.c_str());
        }
        // std::ofstream ofs(filePath.c_str());
        std::ofstream ofs (filePath.c_str(), std::ios::app);

        if (ofs.is_open())
        {
            ofs << "[logger]" << '\t' << logMsg << '\n';
        }
        else
        {
            std::cerr << "Failed to log file : " << errno << std::endl;
        }
#endif
    }
} // namespace Logger

} // namespace onsen
