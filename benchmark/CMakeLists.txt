juce_add_console_app(Os251_Benchmark
        # VERSION ...                                     # Set this if the plugin version is different to the project version
        # ICON_BIG ""   # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL "${CMAKE_SOURCE_DIR}/Assets/icon16.png "
        COMPANY_NAME "Onsen Audio"                    # Specify the name of the plugin's author
        COMPANY_COPYRIGHT "Onsen Audio"
        COMPANY_WEBSITE "https://onsenaudio.com/"
        # COMPANY_EMAIL "not.exist@dummy.com"
        PLUGIN_MANUFACTURER_CODE htMi               # A four-character manufacturer id with at least one upper-case character
        # DESCRIPTION "" # We don't use it. If set, the plug-in name in Pro Tools will be this value.
        MICROPHONE_PERMISSION_ENABLED TRUE
        MICROPHONE_PERMISSION_TEXT "This application requires a permission to use an audio input device of your computer. By Default, Built-In microphone will be used."

        PRODUCT_NAME "OS-251-benchmark")        # The name of the final

target_compile_features(Os251_Benchmark PUBLIC cxx_std_17)

target_link_libraries(Os251_Benchmark PUBLIC
        benchmark)

target_compile_definitions(Os251_Benchmark
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        # JUCE_DISPLAY_SPLASH_SCREEN=0 #if your plugin is distributed with GPL license or paid
        DONT_SET_USING_JUCE_NAMESPACE=1
        )

#juce_add_binary_data(Os251nBinaries
#        SOURCES
#        ${CMAKE_SOURCE_DIR}/assets/juce-logo.svg
#        )

target_sources(Os251_Benchmark PRIVATE
        Main.cpp
        ../src/Chorus.cpp
        ../src/Envelope.cpp
        ../src/SynthEngine.cpp
        ../src/SynthVoice.cpp
        )

target_link_libraries(Os251_Benchmark PUBLIC
        #        Os251Binaries
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

