/*
  ==============================================================================

   Chorus Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "ParamCommon.h"
#include <atomic>
#include <vector>

namespace onsen
{
//==============================================================================
class IChorusParams
{
public:
    virtual bool getChorusOn() const = 0;
};

//==============================================================================
class ChorusParams : public IChorusParams
{
public:
    bool getChorusOn() const override
    {
        return chorusOnVal > 0.5;
    }

    void setChorusOnPtr (std::atomic<flnum>* _chorusOn)
    {
        chorusOn = _chorusOn;
        chorusOnVal = *chorusOn;
    }

    void parameterChanged()
    {
        chorusOnVal = *chorusOn;
    }

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        constexpr int numDecimal = 4;
        return {
            { "chorusOn", "Chorus", 0.0, &chorusOn, ParamUtil::valueToOnOffString }
        };
    }

private:
    std::atomic<flnum>* chorusOn {};
    flnum chorusOnVal = 0.0;
};
} // namespace onsen
