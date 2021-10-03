/*
  ==============================================================================

   Filter Test

  ==============================================================================
*/

#include "../../src/dsp/Filter.h"
#include "../../src/params/EnvelopeParamsMock.h"
#include "../../src/params/FilterParamsMock.h"
#include "../../src/params/LfoParamsMock.h"
#include "util/PositionInfoMock.h"
#include <gtest/gtest.h>

namespace onsen
{
//==============================================================================
// Filter

class FilterTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        env.setCurrentPlaybackSampleRate (sampleRate);
        lfo.setCurrentPlaybackSampleRate (sampleRate);
        lfo.setSamplesPerBlock (samplesPerBlock);
        lfo.renderLfo (0, samplesPerBlock - 1);
        filter.setCurrentPlaybackSampleRate (sampleRate);
        filter.resetBuffer();
    }

    // void TearDown() override {}

    static constexpr double sampleRate = 44100;
    static constexpr flnum samplesPerBlock = 512;
    EnvelopeParamsMock envParams;
    LfoParamsMock lfoParams { 0.5 /*[Hz]*/, 1.0 / 48.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    FilterParamsMock filterParams;
    PositionInfoMock positionInfo;

    Envelope env { &envParams };
    Lfo lfo { &lfoParams, &positionInfo };
    Filter filter { &filterParams, &env, &lfo };
};

TEST_F (FilterTest, Snapshot)
{
    EXPECT_FLOAT_EQ (filter.process (0.3, 0), 1.5171969e-05);
    EXPECT_FLOAT_EQ (filter.process (0.99, 1), 0.00011064461);
    EXPECT_FLOAT_EQ (filter.process (-0.5, 2), 0.00029543752);
}
} // namespace onsen
