/*
  ==============================================================================

   JUCE Plugin Editor for reace-juce

  ==============================================================================
*/

#pragma once

#include "JuceAudioProcessorState.h"
#include "PluginProcessor.h"
#include <JuceHeader.h>
#include <atomic>
#include <memory>
#include <unordered_map>
#include <vector>

//==============================================================================
/**
*/
class Os251AudioProcessorEditor : public juce::AudioProcessorEditor,
                                  public juce::AudioProcessorParameter::Listener,
                                  public juce::Timer
{
public:
    Os251AudioProcessorEditor (Os251AudioProcessor&, juce::AudioProcessorValueTreeState&, onsen::PresetManager&, onsen::ISynthUi* synthUi);
    ~Os251AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void parameterValueChanged (int parameterIndex, float newValue) override;
    void parameterGestureChanged (int parameterIndex, bool gestureIsStarting) override;
    void timerCallback() override;

private:
    juce::File getBundle();
    void setUpParameters();
    void updateUi();
    void beforeBundleEvaluated();
    void afterBundleEvaluated();
    //==============================================================================

    Os251AudioProcessor& audioProcessor;
    onsen::PresetManager& presetManager;
    onsen::ISynthUi* const synthUi;

    std::shared_ptr<reactjuce::EcmascriptEngine> engine;
    reactjuce::ReactApplicationRoot appRoot;
    std::unique_ptr<reactjuce::AppHarness> harness;
    juce::String tmpUiBundlePath;

    std::unordered_map<juce::String, juce::AudioProcessorParameter*> parameterById;
    std::vector<std::atomic<bool>> dirtyParamFlags;

    static constexpr int bodyWidth = 758;
    static constexpr int bodyHeight = 420;
    static constexpr int headerHeight = 32;
    static constexpr int appWidth = bodyWidth;
    static constexpr int appHeight = bodyHeight + headerHeight;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Os251AudioProcessorEditor)
};
