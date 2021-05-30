/*
  ==============================================================================

   Envelope

  ==============================================================================
*/

#include "Envelope.h"

namespace onsen
{
//==============================================================================
void Envelope::noteOn()
{
    sampleCnt = 0;
    state = State::ATTACK;
}

void Envelope::noteOff()
{
    sampleCnt = 0;
    noteOffLevel = level;
    state = State::RELEASE;
}

void Envelope::update()
{
    if (state == State::OFF)
        return;

    if (state == State::ATTACK)
    {
        const flnum attackSec = p->getAttack();
        level = attackCurve (toTimeSec (++sampleCnt), attackSec);
        if (sampleCnt >= toSample (attackSec))
        {
            sampleCnt = 0;
            state = State::DECAY;
        }
    }
    else if (state == State::DECAY)
    {
        const flnum decaySec = p->getDecay();
        const flnum sustain = p->getSustain();
        level = sustain + (MAX_LEVEL - sustain) * decayCurve (toTimeSec (++sampleCnt), decaySec);
        if (sampleCnt >= toSample (decaySec))
        {
            sampleCnt = 0;
            level = sustain;
            state = State::SUSTAIN;
        }
    }
    else if (state == State::SUSTAIN)
    {
        level = p->getSustain();
    }
    else if (state == State::RELEASE)
    {
        const flnum releaseSec = p->getRelease();
        level = noteOffLevel * releaseCurve (toTimeSec (++sampleCnt), releaseSec);
        if (sampleCnt >= toSample (releaseSec))
        {
            sampleCnt = 0;
            level = 0;
            state = State::OFF;
        }
    }
    else
    {
        assert (false && "Unknown state of envelope");
    }
}

//==============================================================================
void Gate::noteOn()
{
    sampleCnt = 0;
    state = State::ATTACK;
}

void Gate::noteOff()
{
    sampleCnt = 0;
    noteOffLevel = level;
    state = State::RELEASE;
}

void Gate::update()
{
    if (state == State::OFF)
        return;

    if (state == State::ATTACK)
    {
        level = attackCurve (toTimeSec (++sampleCnt), attackSec);
        if (sampleCnt >= toSample (attackSec))
        {
            sampleCnt = 0;
            state = State::SUSTAIN;
        }
    }
    else if (state == State::SUSTAIN)
    {
        level = MAX_LEVEL;
    }
    else if (state == State::RELEASE)
    {
        level = noteOffLevel * releaseCurve (toTimeSec (++sampleCnt), releaseSec);
        if (sampleCnt >= toSample (releaseSec))
        {
            sampleCnt = 0;
            level = 0;
        }
    }
    else
    {
        assert (false && "Unknown state of gate");
    }
}
} // namespace onsen
