/*
  ==============================================================================

   SynthParams Mock

  ==============================================================================
*/

#include "../../src/synth/SynthParams.h"
#include <atomic>
#include <memory>

namespace onsen
{
struct SynthParamsMockValues
{
public:
    using flnum = float;
    SynthParamsMockValues() : synthParams (std::make_shared<SynthParams>()),
                              params (synthParams->getParamMetaList().size())
    {
        auto paramMetas = synthParams->getParamMetaList();
        for (int i = 0; i < paramMetas.size(); i++)
        {
            *(paramMetas[i].valuePtr) = &(params[i]);
            **(paramMetas[i].valuePtr) = paramMetas[i].defaultValue;
        }
    }

    std::shared_ptr<SynthParams> getSynthParams()
    {
        return synthParams;
    }

    std::shared_ptr<SynthParams> synthParams;
    std::vector<std::atomic<flnum>> params;
};
} // namespace onsen
