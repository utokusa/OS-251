juce_add_console_app(Os251_Benchmark)

target_compile_features(Os251_Benchmark PUBLIC cxx_std_17)

target_link_libraries(Os251_Benchmark PUBLIC
        benchmark)

target_compile_definitions(Os251_Benchmark
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        DONT_SET_USING_JUCE_NAMESPACE=1
        )

target_sources(Os251_Benchmark PRIVATE
        Main.cpp
        ../src/dsp/Chorus.cpp
        ../src/dsp/Envelope.cpp
        ../src/synth/SynthEngine.cpp
        ../src/synth/SynthVoice.cpp
        )

target_link_libraries(Os251_Benchmark PUBLIC
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_plugin_client
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_dsp
        juce::juce_events
        juce::juce_graphics
        juce::juce_gui_basics
        juce::juce_gui_extra
        )

juce_generate_juce_header(Os251_Benchmark)

