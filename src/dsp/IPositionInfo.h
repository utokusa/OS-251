/*
  ==============================================================================

   IPositionInfo

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"

namespace onsen
{
//==============================================================================
class IPositionInfo
{
public:
    virtual flnum getBpm() const = 0;
    virtual bool isPlaying() const = 0;
    virtual flnum getPpqPosition() const = 0;
};
} // namespace onsen
