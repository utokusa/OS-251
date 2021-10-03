/*
  ==============================================================================

   PositionInfoMock

  ==============================================================================
*/

#pragma once

#include "../../../src/dsp/DspCommon.h"
#include "../../../src/dsp/IPositionInfo.h"

namespace onsen
{
//==============================================================================
class PositionInfoMock : public IPositionInfo
{
public:
    PositionInfoMock() = default;

    flnum getBpm() const override
    {
        return 120;
    }

    bool isPlaying() const override
    {
        return false;
    }

    flnum getPpqPosition() const override
    {
        return 0.0;
    }

private:
};
} // namespace onsen
