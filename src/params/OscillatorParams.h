/*
  ==============================================================================

   Oscillator Parameters

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
class IOscillatorParams
{
public:
    virtual flnum getSinGain() const = 0;
    virtual flnum getSquareGain() const = 0;
    virtual flnum getSawGain() const = 0;
    virtual flnum getSubSquareGain() const = 0;
    virtual flnum getNoiseGain() const = 0;
    virtual flnum getShape() const = 0;
};

//==============================================================================
namespace OscillatorConfig
{
    static constexpr int MAX_NUM_VOICES = 24;
}

//==============================================================================
class OscillatorParams : public IOscillatorParams
{
public:
    //==============================================================================
    flnum getSinGain() const override
    {
        return sinGainVal;
    }

    void setSinGainPtr (std::atomic<flnum>* _sinGain)
    {
        sinGain = _sinGain;
        sinGainVal = *sinGain;
    }

    flnum getSquareGain() const override
    {
        return squareGainVal;
    }

    void setSquareGainPtr (std::atomic<flnum>* _squareGain)
    {
        squareGain = _squareGain;
        squareGainVal = *squareGain;
    }

    flnum getSawGain() const override
    {
        return sawGainVal;
    }

    void setSawGainPtr (std::atomic<flnum>* _sawGain)
    {
        sawGain = _sawGain;
        sawGainVal = *sawGain;
    }

    flnum getSubSquareGain() const override
    {
        return subSquareGainVal;
    }

    void setSubSquareGainPtr (std::atomic<flnum>* _subSquareGain)
    {
        subSquareGain = _subSquareGain;
        subSquareGainVal = *subSquareGain;
    }

    flnum getNoiseGain() const override
    {
        return noiseGainVal;
    }

    void setNoiseGainPtr (std::atomic<flnum>* _noiseGain)
    {
        noiseGain = _noiseGain;
        noiseGainVal = *noiseGain;
    }

    flnum getShape() const override
    {
        return shapeVal;
    }

    void setShapePtr (std::atomic<flnum>* _shape)
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
        shapeVal = *shape;
    }

    std::vector<ParamMetaInfo> getParamMetaList()
    {
        constexpr int numDecimal = 4;
        return {
            { "sinGain", "Sin", 1.0, &sinGain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "squareGain", "Square", 1.0, &squareGain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "sawGain", "Saw", 1.0, &sawGain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "subSquareGain", "SubSquare", 1.0, &subSquareGain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "noiseGain", "Noise", 0.0, &noiseGain, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } },
            { "shape", "Shape", 0.0, &shape, [numDecimal] (float value) { return ParamUtil::valueToString (value, numDecimal); } }
        };
    }

    //==============================================================================
    // For parameters that are not read directly by the synth engine (numVoices and unisonOn)
    static inline BasicParamMetaInfo numVoicesParamBasicMetaInfo()
    {
        constexpr float defaultFlnumNumVoices = 0.285; // The number will be converted to 8. OS-251 has 8 voices as default.
        return { "numVoices", "Num Voices", defaultFlnumNumVoices, [] (float value) {
                    return std::to_string (DspUtil::mapFlnumToInt (value, 0.0, 1.0, 1, OscillatorConfig::MAX_NUM_VOICES));
                } };
    }

    static inline int convertParamValueToNumVoices (float value)
    {
        return DspUtil::mapFlnumToInt (value, 0.0, 1.0, 1, OscillatorConfig::MAX_NUM_VOICES);
    }

    static inline BasicParamMetaInfo unisonOnValueBasicMetaInfo()
    {
        return { "unisonOn", "Unison", 0.0, [] (float value) {
                    return ParamUtil::valueToOnOffString (value);
                } };
    }

    static inline bool convertParamValueToUnisonOn (float value)
    {
        return value > 0.5;
    }

private:
    std::atomic<flnum>* sinGain {};
    std::atomic<flnum>* squareGain {};
    std::atomic<flnum>* sawGain {};
    std::atomic<flnum>* subSquareGain {};
    std::atomic<flnum>* noiseGain {};
    std::atomic<flnum>* shape {};

    flnum sinGainVal = 0.0;
    flnum squareGainVal = 0.0;
    flnum sawGainVal = 0.0;
    flnum subSquareGainVal = 0.0;
    flnum noiseGainVal = 0.0;
    flnum shapeVal = 0.0;
};
} // namespace onsen
