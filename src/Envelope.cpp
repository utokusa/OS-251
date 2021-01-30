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
    state = State::ATTACK;
    constexpr flnum levelNoteStart = MAX_LEVEL * 0.01;
    level = levelNoteStart;
}

void Envelope::noteOFf()
{
    state = State::RELEASE;
}

void Envelope::update()
{
    if (state == State::ATTACK)
    {
        constexpr flnum valFinishAttack = MAX_LEVEL * 0.99;
        // Value of sp.getAttack() is around 0.99
        level = level * MAX_LEVEL / adjust (p->getAttack());
        if (level >= valFinishAttack)
        {
            level = MAX_LEVEL;
            state = State::DECAY;
        }
    }
    else if (state == State::DECAY)
    {
        // Value of sp.getDecay() is around 0.99
        level = level * adjust (p->getDecay());
        flnum sustain = p->getSustain();
        if (level <= sustain)
        {
            level = sustain;
            state = State::SUSTAIN;
        }
    }
    else if (state == State::SUSTAIN)
    {
        // Nothing to do here
    }
    else if (state == State::RELEASE)
    {
        // Value of sp.getRelease() is around 0.99
        level = level * adjust (p->getRelease());
    }
    else
    {
        assert (false && "Unknown state of envelope");
    }
}
}
