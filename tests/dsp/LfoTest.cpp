/*
  ==============================================================================

   Lfo Test

  ==============================================================================
*/

#include "../../src/dsp/Lfo.h"
#include "../../src/params/LfoParamsMock.h"
#include "util/PositionInfoMock.h"
#include <cmath>
#include <gtest/gtest.h>
#include <vector>

namespace onsen
{
//==============================================================================
// LFO

namespace test
{
    constexpr flnum LFO_EPSILON = 0.01;
}

TEST (LfoTest, GetPitchAmount)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 / 48.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    Lfo lfo (&params, &positionInfo);
    EXPECT_FLOAT_EQ (lfo.getPitchAmount(), 0.51);
}

TEST (LfoTest, GetFilterFreqAmount)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 / 48.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    Lfo lfo (&params, &positionInfo);
    EXPECT_FLOAT_EQ (lfo.getFilterFreqAmount(), 0.52);
}

TEST (LfoTest, GetShapeAmount)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 / 48.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    Lfo lfo (&params, &positionInfo);
    EXPECT_FLOAT_EQ (lfo.getShapeAmount(), 0.53);
}

TEST (LfoTest, WaveFormSnapshotWithoutSync)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.renderLfo (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (0), std::sin (0.0), test::LFO_EPSILON);
    EXPECT_NEAR (lfo.getLevel (255), std::sin (pi / 2.0), test::LFO_EPSILON);
    EXPECT_NEAR (lfo.getLevel (511), std::sin (pi), test::LFO_EPSILON);
}

TEST (LfoTest, WaveFormSnapshotWithSync)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, true, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    EXPECT_NEAR (positionInfo.getBpm(), 120.0, EPSILON); // So LFO rate (sync) should be 0.5 [Hz]
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.renderLfoSync (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (0), std::sin (0.0), test::LFO_EPSILON);
    EXPECT_NEAR (lfo.getLevel (255), std::sin (pi / 2.0), test::LFO_EPSILON);
    EXPECT_NEAR (lfo.getLevel (511), std::sin (pi), test::LFO_EPSILON);
}

TEST (LfoTest, LfoOnOff1)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    EXPECT_NEAR (positionInfo.getBpm(), 120.0, EPSILON); // So LFO rate (sync) should be 0.5 [Hz]
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.noteOff(); // Now number of playing notes should be 0.
    lfo.renderLfo (0, 100 /*random number*/); // It changes the value of LFO angle
    lfo.noteOn(); // LFO angle should be reset
    lfo.renderLfo (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (255), std::sin (pi / 2.0), test::LFO_EPSILON);
}

TEST (LfoTest, LfoOnOff2)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, 0.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    EXPECT_NEAR (positionInfo.getBpm(), 120.0, EPSILON); // So LFO rate (sync) should be 0.5 [Hz]
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.noteOn();
    lfo.noteOff();
    lfo.noteOff(); // Now number of playing notes should be 0.
    lfo.renderLfo (0, 100 /*random number*/); // It changes the value of LFO angle
    lfo.noteOn(); // LFO angle should be reset
    lfo.renderLfo (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (255), std::sin (pi / 2.0), test::LFO_EPSILON);
}

TEST (LfoTest, LfoDelay)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, 0.0 /*[rad]*/, 0.99995 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    EXPECT_NEAR (positionInfo.getBpm(), 120.0, EPSILON); // So LFO rate (sync) should be 0.5 [Hz]
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.renderLfo (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (255), 0.029992768540978432, EPSILON);
}

TEST (LfoTest, Phase)
{
    LfoParamsMock params { 0.5 /*[Hz]*/, 1.0 /*[bar]*/, pi / 2.0 /*[rad]*/, 0.0001 /*no unit*/, false, 0.51, 0.52, 0.53 };
    PositionInfoMock positionInfo;
    EXPECT_NEAR (positionInfo.getBpm(), 120.0, EPSILON); // So LFO rate (sync) should be 0.5 [Hz]
    Lfo lfo (&params, &positionInfo);

    // Prepare 1 second buffer
    lfo.setSamplesPerBlock (512);
    lfo.setCurrentPlaybackSampleRate (512.0);

    lfo.noteOn();
    lfo.renderLfo (0, 512);
    // Wave form snapshot test
    EXPECT_NEAR (lfo.getLevel (255), std::sin (pi / 2.0 /*255 th sample*/ + pi / 2.0 /*phase*/), test::LFO_EPSILON);
}
} // namespace onsen
