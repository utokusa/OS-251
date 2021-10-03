/*
  ==============================================================================

   HPF Parameters Mock

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "HpfParams.h"

namespace onsen
{
//==============================================================================
class HpfParamsMock : public IHpfParams
{
public:
    flnum getFrequency() const override
    {
        return 300.0;
    }
};
} // namespace onsen
