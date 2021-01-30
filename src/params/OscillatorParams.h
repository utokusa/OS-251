/*
  ==============================================================================

   Oscillator Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"

namespace onsen
{
//==============================================================================
class OscillatorParams
{
public:
    flnum getSinGain() const
    {
        auto decibelGain = paramValToDecibel (sinGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSinGainPtr (const std::atomic<flnum>* _sinGain)
    {
        sinGain = _sinGain;
        sinGainVal = *sinGain;
    }
    flnum getSquareGain() const
    {
        auto decibelGain = paramValToDecibel (squareGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSquareGainPtr (const std::atomic<flnum>* _squareGain)
    {
        squareGain = _squareGain;
        squareGainVal = *squareGain;
    }
    flnum getSawGain() const
    {
        auto decibelGain = paramValToDecibel (sawGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSawGainPtr (const std::atomic<flnum>* _sawGain)
    {
        sawGain = _sawGain;
        sawGainVal = *sawGain;
    }
    flnum getSubSquareGain() const
    {
        auto decibelGain = paramValToDecibel (subSquareGainVal);
        return decibelToLinear (decibelGain);
    }
    void setSubSquareGainPtr (const std::atomic<flnum>* _subSquareGain)
    {
        subSquareGain = _subSquareGain;
        subSquareGainVal = *subSquareGain;
    }
    flnum getNoiseGain() const
    {
        auto decibelGain = paramValToDecibel (noiseGainVal);
        return decibelToLinear (decibelGain);
    }
    void setNoiseGainPtr (const std::atomic<flnum>* _noiseGain)
    {
        noiseGain = _noiseGain;
        noiseGainVal = *noiseGain;
    }
    void parameterChanged()
    {
        sinGainVal = *sinGain;
        squareGainVal = *squareGain;
        sawGainVal = *sawGain;
        subSquareGainVal = *subSquareGain;
        noiseGainVal = *noiseGain;
    }

private:
    // Dynamic range in [db]
    static constexpr flnum dynamicRange = 48.0;
    const std::atomic<flnum>* sinGain {};
    const std::atomic<flnum>* squareGain {};
    const std::atomic<flnum>* sawGain {};
    const std::atomic<flnum>* subSquareGain {};
    const std::atomic<flnum>* noiseGain {};

    flnum sinGainVal = 0.0;
    flnum squareGainVal = 0.0;
    flnum sawGainVal = 0.0;
    flnum subSquareGainVal = 0.0;
    flnum noiseGainVal = 0.0;

    // Convert parameter value (linear) to gain ([db])
    // in order to make UX better.
    static flnum paramValToDecibel (flnum paramVal)
    {
        // e.g.
        // paramVal: 1.0 ---> gain: 0 [db] (max)
        // paramVal: 0.0 ---> gain: -dynamicRange [db] (min)
        return dynamicRange * (paramVal - 1.0);
    }

    static flnum decibelToLinear (flnum decibelGain)
    {
        return std::pow (10.0, decibelGain / 20.0);
    }
};
}
