/*
  ==============================================================================

   Envelope Parameters

  ==============================================================================
*/

#pragma once

#include "../dsp/DspCommon.h"
#include "ParamCommon.h"
#include <atomic>
#include <vector>

namespace onsen
{
//==============================================================================
class IEnvelopeParams
{
public:
    virtual flnum getAttack() const = 0;
    virtual flnum getDecay() const = 0;
    virtual flnum getSustain() const = 0;
    virtual flnum getRelease() const = 0;
};

//==============================================================================
class EnvelopeParams : public IEnvelopeParams
{
public:
    flnum getAttack() const override
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 2.0;
        return minValSec + attackTimeCurve (attackVal) * (maxValSec - minValSec);
    }

    void setAttackPtr (std::atomic<flnum>* _attack)
    {
        attack = _attack;
        attackVal = *attack;
    }

    flnum getDecay() const override
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 0.5;
        return minValSec + decayTimeCurve (decayVal) * (maxValSec - minValSec);
    }

    void setDecayPtr (std::atomic<flnum>* _decay)
    {
        decay = _decay;
        decayVal = *decay;
    }

    flnum getSustain() const override
    {
        return sustainVal;
    }

    void setSustainPtr (std::atomic<flnum>* _sustain)
    {
        sustain = _sustain;
        sustainVal = *sustain;
    }

    flnum getRelease() const override
    {
        constexpr flnum minValSec = 0.004;
        constexpr flnum maxValSec = 2.0;
        return minValSec + releaseTimeCurve (releaseVal) * (maxValSec - minValSec);
    }

    void setReleasePtr (std::atomic<flnum>* _release)
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

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        constexpr int numDecimal = 4;
        return {
            { "attack", "Attack", 0.0, &attack, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "decay", "Decay", 1.0, &decay, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "sustain", "Sustain", 1.0, &sustain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "release", "Release", 0.5, &release, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } }
        };
    }

private:
    std::atomic<flnum>* attack {};
    std::atomic<flnum>* decay {};
    std::atomic<flnum>* sustain {};
    std::atomic<flnum>* release {};

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
