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
    using CurrentPositionInfo = juce::AudioPlayHead::CurrentPositionInfo;

public:
    JucePositionInfo() = delete;
    JucePositionInfo (CurrentPositionInfo* _positionInfo) : positionInfo (_positionInfo) {}

    flnum getBpm() const override
    {
        return positionInfo->bpm;
    }

    bool isPlaying() const override
    {
        return positionInfo->isPlaying;
    }

    flnum getPpqPosition() const override
    {
        return positionInfo->ppqPosition;
    }

private:
    CurrentPositionInfo* positionInfo;
};
} // namespace onsen
