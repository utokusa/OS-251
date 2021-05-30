/*
  ==============================================================================

   DSP common

  ==============================================================================
*/

#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>

namespace onsen
{
//==============================================================================
using flnum = float;
static constexpr flnum DEFAULT_SAMPLE_RATE = 44100.0;
static constexpr int DEFAULT_SAMPLES_PER_BLOCK = 512;
// TODO: cnage const to capital letters
static constexpr flnum pi = 3.141592653589793238L;
static constexpr flnum EPSILON = std::numeric_limits<flnum>::epsilon();
//==============================================================================
namespace DspUtil
{
    // Map value [0.0, 1.0] to [-1.0, 1.0].
    inline flnum valMinusOneToOne (flnum valZeroToOne)
    {
        return std::clamp<flnum> ((valZeroToOne - 0.5) * 2.0, -1.0, 1.0);
    }

    // Convert parameter value (linear) to gain ([db])
    // in order to make UX better.
    inline flnum paramValToDecibel (flnum paramVal, flnum dynamicRange)
    {
        // e.g.
        // paramVal: 1.0 ---> gain: 0 [db] (max)
        // paramVal: 0.0 ---> gain: -dynamicRange [db] (min)
        return dynamicRange * (paramVal - 1.0);
    }

    inline flnum decibelToParamVal (flnum decibelGain, flnum dynamicRange)
    {
        return decibelGain / dynamicRange + 1.0;
    }

    inline flnum decibelToLinear (flnum decibelGain)
    {
        return std::pow (10.0, decibelGain / 20.0);
    }

    inline int mapFlnumToInt (flnum fval, flnum fmin, flnum fmax, int imin, int imax)
    {
        // fmin ---> mininum number of floating point number
        // imax ---> maximum number of integer
        assert (fval >= fmin);
        assert (fmax > fmin);
        assert (imax > imin);
        return static_cast<int> ((fval - fmin) / (fmax - fmin) * (imax - imin) + 0.5) + imin;
    }

    inline int timeSecToSample (flnum timeSec, double sampleRate)
    {
        return timeSec * sampleRate;
    }

    inline flnum sampleToTimeSec (int sample, double sampleRate)
    {
        return sample / sampleRate;
    }
} // namespace DspUtil

class SmoothFlnum
{
public:
    SmoothFlnum (flnum val, flnum _smoothness)
        : sampleRate (DEFAULT_SAMPLE_RATE),
          target (val),
          cur (val),
          smoothness (_smoothness),
          adjustedSmoothness (_smoothness) {}
    flnum get() { return cur = adjustedSmoothness * cur + (1 - adjustedSmoothness) * target; }
    void set (flnum val) { target = val; }
    void reset (flnum val)
    {
        target = val;
        cur = val;
    }
    void setSmoothness (flnum val)
    {
        smoothness = val;
        adjustedSmoothness = adjust (smoothness);
    }
    void prepareToPlay (double _sampleRate)
    {
        sampleRate = _sampleRate;
        adjustedSmoothness = adjust (smoothness);
    }

private:
    flnum sampleRate;
    flnum target;
    flnum cur;
    flnum smoothness;
    flnum adjustedSmoothness;

    // Adjust parameter value like attack, decay or release according to the
    // sampling rate
    flnum adjust (const flnum val) const
    {
        // If no need to adjust
        if (std::abs (sampleRate - DEFAULT_SAMPLE_RATE) <= EPSILON)
        {
            return val;
        }
        const flnum amount = std::pow (val, DEFAULT_SAMPLE_RATE / sampleRate - 1);
        return val * amount;
    }
};

namespace ZeroOneToZeroOne
{
    [[maybe_unused]] inline flnum linear (flnum x)
    {
        return x;
    }

    inline flnum square (flnum x)
    {
        x = std::clamp (x, 0.0f, 1.0f);
        return x * x;
    }

    [[maybe_unused]] inline flnum invert (flnum x)
    {
        x = std::clamp (x, 0.0f, 1.0f);
        return x * x;
    }

    [[maybe_unused]] inline flnum tanh (flnum x)
    {
        x = std::clamp (x, 0.0f, 1.0f);
        return std::tanh ((x - 0.5) * 2 * pi) * 0.5 + 0.5;
    }
} // namespace ZeroOneToZeroOne

} // namespace onsen
