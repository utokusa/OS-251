cmake_minimum_required(VERSION 3.20)
include(GoogleTest)

# Tests
add_executable(Os251_Tests)

target_compile_features(Os251_Tests PUBLIC cxx_std_17)

target_link_libraries(Os251_Tests PUBLIC
        gtest_main)

target_sources(Os251_Tests PRIVATE
        dsp/ChorusTest.cpp
        dsp/EnvelopeTest.cpp
        dsp/OscillatorTest.cpp
        dsp/LfoTest.cpp
        dsp/FilterTest.cpp
        dsp/HpfTest.cpp
        dsp/MasterVolumeTest.cpp
        dsp/util/TestAudioBufferInput.cpp
        synth/SynthEngineTest.cpp
        ../src/dsp/Chorus.cpp
        ../src/dsp/Envelope.cpp
        ../src/synth/SynthVoice.cpp
        ../src/synth/SynthEngine.cpp
        )

gtest_discover_tests(Os251_Tests)

# Tests using JUCE
juce_add_console_app(Os251_TestsUsingJuce)

target_link_libraries(Os251_TestsUsingJuce PUBLIC
        gtest_main)

target_compile_definitions(Os251_TestsUsingJuce
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        DONT_SET_USING_JUCE_NAMESPACE=1
        )

target_sources(Os251_TestsUsingJuce PRIVATE
        ../src/services/PresetManager.cpp
        services/PresetManagerTest.cpp
        services/TmpFileManagerTest.cpp
        )

target_link_libraries(Os251_TestsUsingJuce PUBLIC
        Os251Binaries
        juce::juce_audio_processors
        juce::juce_core
        )

# Avoid link error on linux + gcc
# https://forum.juce.com/t/loading-pytorch-model-using-binarydata/39997/2
set_target_properties(Os251Binaries PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
        )

juce_generate_juce_header(Os251_TestsUsingJuce)

gtest_discover_tests(Os251_TestsUsingJuce)
