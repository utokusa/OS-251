/*
  ==============================================================================

   IAudioBuffer

  ==============================================================================
*/

#pragma once

#include "DspCommon.h"

namespace onsen
{
//==============================================================================
class IAudioBuffer
{
public:
    virtual int getNumChannels() const noexcept = 0;
    virtual int getNumSamples() const noexcept = 0;
    virtual flnum* getWritePointer (int channel) noexcept = 0;
    virtual flnum getSample (int channel, int sampleIndex) const noexcept = 0;
    virtual void setSample (int destChannel, int destSample, flnum newValue) noexcept = 0;
};
} // namespace onsen
