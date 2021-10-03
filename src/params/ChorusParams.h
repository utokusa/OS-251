/*
  ==============================================================================

   Chorus Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include <atomic>

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
    void setChorusOnPtr (const std::atomic<flnum>* _chorusOn)
    {
        chorusOn = _chorusOn;
        chorusOnVal = *chorusOn;
    }
    void parameterChanged()
    {
        chorusOnVal = *chorusOn;
    }

private:
    const std::atomic<flnum>* chorusOn {};
    flnum chorusOnVal = 0.0;
};
} // namespace onsen
