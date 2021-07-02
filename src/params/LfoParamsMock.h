/*
  ==============================================================================

   LFO Parameters Mcok

  ==============================================================================
*/

#pragma once

#include "LfoParams.h"

namespace onsen
{
struct LfoParamsMock : public ILfoParams
{
    LfoParamsMock (
        flnum rate,
        flnum rateSync,
        flnum phase,
        flnum delay,
        bool syncOn,
        flnum pitch,
        flnum filterFreq,
        flnum shape) : rate (rate),
                       rateSync (rateSync),
                       phase (phase),
                       delay (delay),
                       syncOn (syncOn),
                       pitch (pitch),
                       filterFreq (filterFreq),
                       shape (shape) {}

    flnum getRate() const override { return rate; }
    flnum getRateSync() const override { return rateSync; }
    flnum getPhase() const override { return phase; }
    flnum getDelay() const override { return delay; }
    bool getSyncOn() const override { return syncOn; }
    flnum getPitch() const override { return pitch; }
    flnum getFilterFreq() const override { return filterFreq; }
    flnum getShape() const override { return shape; }

    flnum rate;
    flnum rateSync;
    flnum phase;
    flnum delay;
    bool syncOn;
    flnum pitch;
    flnum filterFreq;
    flnum shape;
};

} // namespace onsen
