/*
  ==============================================================================

   Test audio buffer input

  ==============================================================================
*/

#include "../../../src/dsp/DspCommon.h"
#include "../../../src/dsp/IAudioBuffer.h"

namespace onsen
{
void setTestInput1 (IAudioBuffer* audioBuffer);
void setTestInput2Constant (IAudioBuffer* audioBuffer, flnum val);
} // namespace onsen
