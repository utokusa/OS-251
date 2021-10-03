/*
  ==============================================================================

   Factory Presets

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <array>

namespace onsen
{
//==============================================================================
struct BinaryPreset
{
    const juce::String path;
    const char* data;
    const int size;
};
//==============================================================================
static std::array<BinaryPreset, 16> factoryPresets = { {

    { "Bass/Bass0.oapreset", BinaryData::Bass0_oapreset, BinaryData::Bass0_oapresetSize },
    { "Bass/Bass1.oapreset", BinaryData::Bass1_oapreset, BinaryData::Bass1_oapresetSize },
    { "Bass/Bass2.oapreset", BinaryData::Bass2_oapreset, BinaryData::Bass2_oapresetSize },
    { "Bass/Bass3.oapreset", BinaryData::Bass3_oapreset, BinaryData::Bass3_oapresetSize },

    { "FX/FX0.oapreset", BinaryData::FX0_oapreset, BinaryData::FX0_oapresetSize },
    { "FX/FX1.oapreset", BinaryData::FX1_oapreset, BinaryData::FX1_oapresetSize },
    { "FX/FX2.oapreset", BinaryData::FX2_oapreset, BinaryData::FX2_oapresetSize },
    { "FX/FX3.oapreset", BinaryData::FX3_oapreset, BinaryData::FX3_oapresetSize },

    { "Key/Key0.oapreset", BinaryData::Key0_oapreset, BinaryData::Key0_oapresetSize },
    { "Key/Key1.oapreset", BinaryData::Key1_oapreset, BinaryData::Key1_oapresetSize },
    { "Key/Key2.oapreset", BinaryData::Key2_oapreset, BinaryData::Key2_oapresetSize },
    { "Key/Key3.oapreset", BinaryData::Key3_oapreset, BinaryData::Key3_oapresetSize },

    { "Pad/Pad0.oapreset", BinaryData::Pad0_oapreset, BinaryData::Pad0_oapresetSize },
    { "Pad/Pad1.oapreset", BinaryData::Pad1_oapreset, BinaryData::Pad1_oapresetSize },
    { "Pad/Pad2.oapreset", BinaryData::Pad2_oapreset, BinaryData::Pad2_oapresetSize },
    { "Pad/Pad3.oapreset", BinaryData::Pad3_oapreset, BinaryData::Pad3_oapresetSize }

} };

} // namespace onsen
