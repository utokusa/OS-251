/*
  ==============================================================================

   Filter Parameters Mock

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "FilterParams.h"

namespace onsen
{
//==============================================================================
class FilterParamsMock : public IFilterParams
{
public:
    flnum getControlledFrequency (flnum controlVal) const override
    {
        return 100.0;
    }

    flnum getResonance() const override
    {
        return 2.0;
    }

    flnum getFilterEnvelope() const override
    {
        return 0.5f;
    }
};
} // namespace onsen
