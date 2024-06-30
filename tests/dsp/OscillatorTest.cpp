/*
  ==============================================================================

   Oscillator Test

  ==============================================================================
*/

#include "../../src/dsp/Oscillator.h"
#include "../../src/params/OscillatorParamsMock.h"
#include <gtest/gtest.h>
#include <vector>

namespace onsen
{
//==============================================================================
// Oscillator
// TODO: Add tests for anti-aliasing

TEST (OscillatorTest, Sin)
{
    // Only sin oscillator is used
    OscillatorParamsMock params { 1.0, 0.0, 0.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that sin's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0, 0.5 * pi), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0, 0.5 * pi), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0, 0.5 * pi), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0, 0.5 * pi), 0.5, EPSILON);
}

TEST (OscillatorTest, Square)
{
    // Only square oscillator is used
    OscillatorParamsMock params { 0.0, 1.0, 0.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that square's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0, 0.5 * pi), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0, 0.5 * pi), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0, 0.5 * pi), 1.0, EPSILON);
}

TEST (OscillatorTest, Saw)
{
    // Only saw oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 1.0, 0.0, 0.0, 0.0 };
    Oscillator osc (&params);
    // Note that saw's algle is twice angleRad parameter of ocillatorVal()
    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0) / 2.0, 0.0, 0.5 * pi), -0.5, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi) / 2.0, 0.0, 0.5 * pi), 0.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0) / 2.0, 0.0, 0.5 * pi), 0.5, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0) / 2.0, 0.0, 0.5 * pi), -0.83333331346511841, EPSILON);
}

TEST (OscillatorTest, SubSquare)
{
    // Only sub square oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 0.0, 1.0, 0.0, 0.0 };
    Oscillator osc (&params);

    EXPECT_NEAR (osc.oscillatorVal (0.0, 0.0, 0.5 * pi), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 2.0), 0.0, 0.5 * pi), 1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi), 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi * 3.0 / 2.0), 0.0, 0.5 * pi), -1.0, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0, 0.5 * pi), 1.0, EPSILON);
}

TEST (OscillatorTest, Noise)
{
    // Only noise oscillator is used
    OscillatorParamsMock params { 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 };
    Oscillator osc (&params);

    int n = 1000;
    std::vector<flnum> vals;
    flnum mean = 0.0;
    // Generate value and calculate mean
    for (int i = 0; i < n; ++i)
    {
        flnum val = osc.oscillatorVal (0.0, 0.0, 0.5 * pi);
        EXPECT_LE (val, 1.0);
        EXPECT_GE (val, 0.0);
        vals.push_back (val);
        mean += val;
    }
    mean /= n;

    constexpr flnum LAX_EPSILON = 0.1;
    EXPECT_NEAR (mean, 0.5, LAX_EPSILON);

    // Calculate variance
    flnum variance = 0.0;
    for (auto val : vals)
    {
        variance += std::pow ((val - mean), 2.0);
    }
    variance /= n;

    EXPECT_LT (variance, 0.5);
    EXPECT_GT (variance, 0.0);

    // EXPECT_NEAR(osc.oscillatorVal(0.0, 0.0), 1.0, EPSILON);
}

TEST (OscillatorTest, ChangeShapeAndMixWaves)
{
    // Start with shape = 0
    OscillatorParamsMock params { 1.0, 1.0, 1.0, 1.0, 0.0, 0.0 };
    Oscillator osc (&params);
    constexpr int NUM_UPDATE = 5000;
    // Use lax epsilon because shape value is smoothed
    constexpr flnum LAX_EPSILON = 0.001;

    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0, 0.5 * pi), 2.1993587017059326, EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0, 0.5 * pi), 2.5326919555664062, EPSILON);
    params.shape = 0.5;
    // Wait for oscillatorVal becames stable
    for (int i = 0; i < NUM_UPDATE; i++)
    {
        osc.oscillatorVal (0, 0.0, 0.5 * pi);
    }
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0, 0.5 * pi), 1.6666688919067383, LAX_EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0, 0.5 * pi), 2.1997506618499756, LAX_EPSILON);
    params.shape = 1.0;
    // Wait for oscillatorVal becames stable
    for (int i = 0; i < NUM_UPDATE; i++)
    {
        osc.oscillatorVal (0, 0.0, 0.5 * pi);
    }
    EXPECT_NEAR (osc.oscillatorVal ((pi / 6.0), 0.0, 0.5 * pi), 1.0000048875808716, LAX_EPSILON);
    EXPECT_NEAR (osc.oscillatorVal ((pi / 3.0), 0.0, 0.5 * pi), 1.0012624263763428, LAX_EPSILON);
}
} // namespace onsen
