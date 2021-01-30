/*
  ==============================================================================

   Chorus Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"

namespace onsen
{
//==============================================================================
class ChorusParams
{
public:
    bool getChorusOn() const
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
}
