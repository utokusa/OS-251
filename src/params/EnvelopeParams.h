/*
  ==============================================================================

   Envelope Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class EnvelopeParams
{
public:
    flnum getAttack() const
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 2.0;
        return minValSec + attackTimeCurve (attackVal) * (maxValSec - minValSec);
    }
    void setAttackPtr (const std::atomic<flnum>* _attack)
    {
        attack = _attack;
        attackVal = *attack;
    }
    flnum getDecay() const
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 0.5;
        return minValSec + decayTimeCurve (decayVal) * (maxValSec - minValSec);
    }
    void setDecayPtr (const std::atomic<flnum>* _decay)
    {
        decay = _decay;
        decayVal = *decay;
    }
    flnum getSustain() const
    {
        return sustainVal;
    }
    void setSustainPtr (const std::atomic<flnum>* _sustain)
    {
        sustain = _sustain;
        sustainVal = *sustain;
    }
    flnum getRelease() const
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 2.0;
        return minValSec + releaseTimeCurve (releaseVal) * (maxValSec - minValSec);
    }
    void setReleasePtr (const std::atomic<flnum>* _release)
    {
        release = _release;
        releaseVal = *release;
    }
    void parameterChanged()
    {
        attackVal = *attack;
        decayVal = *decay;
        sustainVal = *sustain;
        releaseVal = *release;
    }

private:
    const std::atomic<flnum>* attack {};
    const std::atomic<flnum>* decay {};
    const std::atomic<flnum>* sustain {};
    const std::atomic<flnum>* release {};

    flnum attackVal = 0.0;
    flnum decayVal = 0.0;
    flnum sustainVal = 0.0;
    flnum releaseVal = 0.0;

    // Map flnum value [0, 1] to [0, 1]
    flnum attackTimeCurve (flnum val0to1) const
    {
        // return std::pow(val0to1, 1.0 / 10.0);
        return val0to1;
    }

    // Map flnum value [0, 1] to [0, 1]
    flnum decayTimeCurve (flnum val0to1) const
    {
        return val0to1;
    }

    // Map flnum value [0, 1] to [0, 1]
    flnum releaseTimeCurve (flnum val0to1) const
    {
        return val0to1;
    }
};
} // namespace onsen
