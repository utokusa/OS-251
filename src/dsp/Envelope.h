/*
  ==============================================================================

   Envelope

  ==============================================================================
*/

#pragma once

#include "../synth/SynthParams.h"
#include "DspCommon.h"

namespace onsen
{
//==============================================================================
class IEnvelope
{
public:
    enum class State
    {
        OFF,
        ATTACK,
        DECAY,
        SUSTAIN,
        RELEASE
    };
    virtual ~IEnvelope() = default;
    virtual void noteOn() = 0;
    virtual void noteOff() = 0;
    virtual void update() = 0;
    virtual flnum getLevel() const = 0;
    virtual flnum isEnvOff() const = 0;
    virtual void setCurrentPlaybackSampleRate (const double newRate) = 0;
};

//==============================================================================
class Envelope : public IEnvelope
{
public:
    Envelope() = delete;
    Envelope (IEnvelopeParams* const envParams)
        : p (envParams),
          sampleRate (DEFAULT_SAMPLE_RATE),
          state (State::OFF),
          level (0.0),
          noteOffLevel (0.0),
          sampleCnt (0) {}

    void noteOn() override;
    void noteOff() override;
    void update() override;
    flnum getLevel() const override { return level; }
    flnum isEnvOff() const override { return state == State::OFF; }
    void setCurrentPlaybackSampleRate (const double newRate) override { sampleRate = newRate; }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    const IEnvelopeParams* const p;
    flnum sampleRate;

    State state;
    flnum level;
    flnum noteOffLevel;
    int sampleCnt;

    // Return value [0, 1]
    flnum attackCurve (flnum curTimeSec, flnum lengthSec)
    {
        return curTimeSec / lengthSec;
    }

    // Return value [0, 1]
    flnum decayCurve (flnum curTimeSec, flnum lengthSec)
    {
        return ZeroOneToZeroOne::square ((lengthSec - curTimeSec) / lengthSec);
    }

    // Return value [0, 1]
    flnum releaseCurve (flnum curTimeSec, flnum lengthSec)
    {
        return (lengthSec - curTimeSec) / lengthSec;
    }
};

//==============================================================================
// Gate does not have ADSR but it has fixed attack and release around 1 [ms] instead.
class Gate : public IEnvelope
{
public:
    Gate()
        : sampleRate (DEFAULT_SAMPLE_RATE),
          state (State::OFF),
          level (0.0),
          noteOffLevel (0.0),
          sampleCnt (0) {}

    void noteOn() override;
    void noteOff() override;
    void update() override;
    flnum getLevel() const override { return level; }
    flnum isEnvOff() const override { return state == State::OFF; }
    void setCurrentPlaybackSampleRate (const double newRate) override { sampleRate = newRate; }

private:
    static constexpr flnum MAX_LEVEL = 1.0;

    flnum sampleRate;

    State state;
    flnum level;
    flnum noteOffLevel;
    int sampleCnt;

    static constexpr flnum attackSec = 0.002; // [s]
    static constexpr flnum releaseSec = 0.002; // [s]

    // Return value [0, 1]
    flnum attackCurve (flnum curTimeSec, flnum lengthSec)
    {
        return curTimeSec / lengthSec;
    }

    // Return value [0, 1]
    flnum releaseCurve (flnum curTimeSec, flnum lengthSec)
    {
        return (lengthSec - curTimeSec) / lengthSec;
    }
};

//==============================================================================
// It allows you to switch between Gate and Envelope
class EnvManager : public IEnvelope
{
public:
    EnvManager() = delete;
    EnvManager (Envelope* _env, Gate* _gate)
        : env (_env),
          gate (_gate),
          target (env) {}

    void noteOn() override
    {
        env->noteOn();
        gate->noteOn();
    };
    void noteOff() override
    {
        env->noteOff();
        gate->noteOff();
    };
    void update() override
    {
        env->update();
        gate->update();
    };
    flnum getLevel() const override { return target->getLevel(); }
    flnum isEnvOff() const override { return target->isEnvOff(); }
    void setCurrentPlaybackSampleRate (const double newRate) override
    {
        env->setCurrentPlaybackSampleRate (newRate);
        gate->setCurrentPlaybackSampleRate (newRate);
    }

    void switchTarget (bool useEnvelope)
    {
        target = useEnvelope ? env : gate;
    }

private:
    IEnvelope* env;
    IEnvelope* gate;
    IEnvelope* target;
};

} // namespace onsen
