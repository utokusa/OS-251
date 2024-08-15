/*
  ==============================================================================

   JucePositionInfo

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "../dsp/IPositionInfo.h"
#include <JuceHeader.h>

namespace onsen
{
//==============================================================================
class JucePositionInfo : public IPositionInfo
{
    using CurrentPositionInfo = juce::AudioPlayHead::PositionInfo;

public:
    JucePositionInfo() = delete;
    JucePositionInfo (CurrentPositionInfo* _positionInfo) : positionInfo (_positionInfo) {}

    flnum getBpm() const override
    {
        return positionInfo->getBpm().orFallback (120.0);
    }

    bool isPlaying() const override
    {
        return positionInfo->getIsPlaying();
    }

    flnum getPpqPosition() const override
    {
        return positionInfo->getPpqPosition().orFallback (0.0);
    }

private:
    CurrentPositionInfo* positionInfo;
};
} // namespace onsen
