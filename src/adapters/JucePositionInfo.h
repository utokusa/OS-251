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
        // TODO: fix optional
        return *positionInfo->getBpm();
    }

    bool isPlaying() const override
    {
        return positionInfo->getIsPlaying();
    }

    flnum getPpqPosition() const override
    {
        // TODO: fix optional
        return *positionInfo->getPpqPosition();
    }

private:
    CurrentPositionInfo* positionInfo;
};
} // namespace onsen
