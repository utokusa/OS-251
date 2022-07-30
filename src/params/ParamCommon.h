/*
  ==============================================================================

   Param common

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include <functional>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

namespace onsen
{
//==============================================================================
namespace ParamUtil
{
    inline std::string floatToString (float value, int numDecimal)
    {
        // Avoid displaying minus zeros as "-0.0 ..."
        // https://en.wikipedia.org/wiki/Signed_zero
        value += 0.0f;
        std::ostringstream ss;
        ss << std::fixed << std::setprecision (numDecimal) << value;
        return ss.str();
    }

    inline std::string valueToString (float value, int numDecimal)
    {
        return floatToString (value, numDecimal);
    }

    inline std::string valueToMinusOneToOneString (float value, int numDecimal)
    {
        return floatToString (onsen::DspUtil::valMinusOneToOne (value), numDecimal);
    }

    inline std::string valueToOnOffString (float value)
    {
        return value > 0.5 ? "ON" : "OFF";
    }

    inline std::string valueToFreqString (float value, float lowestFreqVal, float freqBaseNumber)
    {
        return std::to_string ((int) (pow (freqBaseNumber, value) * lowestFreqVal)) + " Hz";
    };

    inline std::string valueToResString (float value, float lowestResVal, float resBaseNumber, int numDecimal)
    {
        return floatToString (pow (resBaseNumber, value) * lowestResVal, numDecimal);
    }
} // namespace ParamUtil

//==============================================================================
struct BasicParamMetaInfo
{
public:
    std::string paramId;
    std::string paramName;
    flnum defaultValue;
    std::function<std::string (flnum)> valueToString;
};

struct ParamMetaInfo
{
public:
    std::string paramId;
    std::string paramName;
    flnum defaultValue;
    std::atomic<flnum>** valuePtr;
    std::function<std::string (flnum)> valueToString;
};

} // namespace onsen
