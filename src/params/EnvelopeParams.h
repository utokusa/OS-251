/*
  ==============================================================================

   Envelope Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"

namespace onsen
{
//==============================================================================
class EnvelopeParams
{
public:
    flnum getAttack() const
    {
        constexpr flnum minVal = 0.995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (attackVal) * (maxVal - minVal);
    }
    void setAttackPtr (const std::atomic<flnum>* _attack)
    {
        attack = _attack;
        attackVal = *attack;
    }
    flnum getDecay() const
    {
        constexpr flnum minVal = 0.9995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (decayVal) * (maxVal - minVal);
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
        constexpr flnum minVal = 0.995;
        constexpr flnum maxVal = 0.99999;
        return minVal + (releaseVal) * (maxVal - minVal);
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
};
}
