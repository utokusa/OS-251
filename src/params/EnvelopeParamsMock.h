/*
  ==============================================================================

   Envelope Parameters Mock

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "EnvelopeParams.h"

namespace onsen
{
//==============================================================================
class EnvelopeParamsMock : public IEnvelopeParams
{
public:
    flnum getAttack() const override
    {
        return 3.0 - EPSILON; // [sec]
    }
    flnum getDecay() const override
    {
        return 3.0 - EPSILON; // [sec]
    }
    flnum getSustain() const override
    {
        return 0.5 - EPSILON;
    }
    flnum getRelease() const override
    {
        return 3.0 - EPSILON; // [sec]
    }
};
} // namespace onsen
