/*
  ==============================================================================

   Oscillator Parameters

  ==============================================================================
*/

#pragma once

#include "../DspCommon.h"
#include <atomic>

namespace onsen
{
//==============================================================================
class IOscillatorParams
{
public:
    virtual flnum getSinGain() const = 0;
    virtual flnum getSquareGain() const = 0;
    virtual flnum getSawGain() const = 0;
    virtual flnum getSubSquareGain() const = 0;
    virtual flnum getNoiseGain() const = 0;
    virtual flnum getShape() = 0;
};

//==============================================================================
class OscillatorParams : public IOscillatorParams
{
public:
    // Dynamic range in [db]
    static constexpr flnum dynamicRange = 48.0;

    //==============================================================================
    flnum getSinGain() const override
    {
        auto decibelGain = DspUtil::paramValToDecibel (sinGainVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setSinGainPtr (const std::atomic<flnum>* _sinGain)
    {
        sinGain = _sinGain;
        sinGainVal = *sinGain;
    }
    flnum getSquareGain() const override
    {
        auto decibelGain = DspUtil::paramValToDecibel (squareGainVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setSquareGainPtr (const std::atomic<flnum>* _squareGain)
    {
        squareGain = _squareGain;
        squareGainVal = *squareGain;
    }
    flnum getSawGain() const override
    {
        auto decibelGain = DspUtil::paramValToDecibel (sawGainVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setSawGainPtr (const std::atomic<flnum>* _sawGain)
    {
        sawGain = _sawGain;
        sawGainVal = *sawGain;
    }
    flnum getSubSquareGain() const override
    {
        auto decibelGain = DspUtil::paramValToDecibel (subSquareGainVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setSubSquareGainPtr (const std::atomic<flnum>* _subSquareGain)
    {
        subSquareGain = _subSquareGain;
        subSquareGainVal = *subSquareGain;
    }
    flnum getNoiseGain() const override
    {
        auto decibelGain = DspUtil::paramValToDecibel (noiseGainVal, dynamicRange);
        return DspUtil::decibelToLinear (decibelGain);
    }
    void setNoiseGainPtr (const std::atomic<flnum>* _noiseGain)
    {
        noiseGain = _noiseGain;
        noiseGainVal = *noiseGain;
    }
    flnum getShape() override
    {
        return shapeVal.get();
    }
    void setShapePtr (const std::atomic<flnum>* _shape)
    {
        shape = _shape;
    }
    void parameterChanged()
    {
        sinGainVal = *sinGain;
        squareGainVal = *squareGain;
        sawGainVal = *sawGain;
        subSquareGainVal = *subSquareGain;
        noiseGainVal = *noiseGain;
        shapeVal.set (*shape);
    }
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate)
    {
        shapeVal.prepareToPlay (sampleRate);
    }

private:
    const std::atomic<flnum>* sinGain {};
    const std::atomic<flnum>* squareGain {};
    const std::atomic<flnum>* sawGain {};
    const std::atomic<flnum>* subSquareGain {};
    const std::atomic<flnum>* noiseGain {};
    const std::atomic<flnum>* shape {};

    flnum sinGainVal = 0.0;
    flnum squareGainVal = 0.0;
    flnum sawGainVal = 0.0;
    flnum subSquareGainVal = 0.0;
    flnum noiseGainVal = 0.0;
    SmoothFlnum shapeVal = { 0.0, 0.995 };
};
} // namespace onsen
