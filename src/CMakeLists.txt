if(DEFINED ENV{AAX_SDK_PATH})
    set(AAX_SDK_PATH $ENV{AAX_SDK_PATH})
endif()
if(DEFINED AAX_SDK_PATH)
    set(AAX_BUILD_FLAG AAX)
    file(TO_CMAKE_PATH "${AAX_SDK_PATH}" AAX_SDK_CMAKE_PATH)
    message(STATUS "AAX Build is enabled. AAX SDK = " ${AAX_SDK_CMAKE_PATH})
    juce_set_aax_sdk_path(${AAX_SDK_CMAKE_PATH})
else()
    message(STATUS "AAX Build is disabled. To enable, set AAX_SDK_PATH to your environment variable or specify -DAAX_SDK_PATH to CMake configure option.")
endif()

# default plugin formats
set(JUCE_FORMATS AU VST3 Standalone ${AAX_BUILD_FLAG})
# Build LV2 only on Linux
if(UNIX AND NOT APPLE)
    message(STATUS "Building LV2 plugin format")
    list(APPEND JUCE_FORMATS LV2)
endif()

juce_add_plugin(Os251
        # VERSION ...                                     # Set this if the plugin version is different to the project version
        # ICON_BIG ""   # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
        # ICON_SMALL "${CMAKE_SOURCE_DIR}/Assets/icon16.png "
        COMPANY_NAME "Onsen Audio"                    # Specify the name of the plugin's author
        COMPANY_COPYRIGHT "Onsen Audio"
        COMPANY_WEBSITE "https://onsenaudio.com/"
        # COMPANY_EMAIL "not.exist@dummy.com"
        IS_SYNTH TRUE                       # Is this a synth or an effect?
        NEEDS_MIDI_INPUT TRUE               # Does the plugin need midi input?
        # NEEDS_MIDI_OUTPUT TRUE/FALSE              # Does the plugin need midi output?
        # IS_MIDI_EFFECT TRUE/FALSE                 # Is this plugin a MIDI effect?
        # EDITOR_WANTS_KEYBOARD_FOCUS TRUE/FALSE    # Does the editor need keyboard focus?
        COPY_PLUGIN_AFTER_BUILD TRUE        # Should the plugin be installed to a default location after building?
        PLUGIN_MANUFACTURER_CODE htMi               # A four-character manufacturer id with at least one upper-case character
        PLUGIN_CODE osSY                            # A unique four-character plugin id with at least one upper-case character
        FORMATS ${JUCE_FORMATS}
        VST3_CATEGORIES "Instrument"
        AU_MAIN_TYPE "kAudioUnitType_MusicDevice"
        # AU_SANDBOX_SAFE TRUE
        # AAX_CATEGORY ""
        LV2_URI https://github.com/utokusa/OS-251
        LV2_SHARED_LIBRARY_NAME os251lv2
        # HARDENED_RUNTIME_ENABLED # macOS app settings
        # HARDENED_RUNTIME_OPTIONS
        # APP_SANDBOX_ENABLED
        # APP_SANDBOX_INHERIT
        # DESCRIPTION "" # We don't use it. If set, the plug-in name in Pro Tools will be this value.
        MICROPHONE_PERMISSION_ENABLED TRUE
        MICROPHONE_PERMISSION_TEXT "This application requires a permission to use an audio input device of your computer. By Default, Built-In microphone will be used."

        PRODUCT_NAME "OS-251")        # The name of the final

target_compile_features(Os251 PUBLIC cxx_std_17)


target_compile_definitions(Os251
        PUBLIC
        OS251_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}"
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_DISPLAY_SPLASH_SCREEN=0 #if your plugin is distributed with GPL license or paid
        DONT_SET_USING_JUCE_NAMESPACE=1
        )

set(BINARY_SOURCES
    ${CMAKE_SOURCE_DIR}/src/jsui/build/js/main.js
    ${CMAKE_SOURCE_DIR}/assets/Recursive-Regular.ttf

    ${CMAKE_SOURCE_DIR}/assets/images/left.png
    ${CMAKE_SOURCE_DIR}/assets/images/left_over.png
    ${CMAKE_SOURCE_DIR}/assets/images/left_down.png

    ${CMAKE_SOURCE_DIR}/assets/images/right.png
    ${CMAKE_SOURCE_DIR}/assets/images/right_over.png
    ${CMAKE_SOURCE_DIR}/assets/images/right_down.png

    ${CMAKE_SOURCE_DIR}/assets/images/reload.png
    ${CMAKE_SOURCE_DIR}/assets/images/reload_over.png
    ${CMAKE_SOURCE_DIR}/assets/images/reload_down.png

    ${CMAKE_SOURCE_DIR}/assets/presets/Default.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Bass/Bass0.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Bass/Bass1.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Bass/Bass2.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Bass/Bass3.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/FX/FX0.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/FX/FX1.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/FX/FX2.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/FX/FX3.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Key/Key0.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Key/Key1.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Key/Key2.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Key/Key3.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Pad/Pad0.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Pad/Pad1.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Pad/Pad2.oapreset
    ${CMAKE_SOURCE_DIR}/assets/presets/Factory/Pad/Pad3.oapreset
)

if (UNIX)
    list(APPEND BINARY_SOURCES ${CMAKE_SOURCE_DIR}/assets/Recursive-Regular.ttf)
else()
    list(APPEND BINARY_SOURCES ${CMAKE_SOURCE_DIR}/assets/Os251Font125Percent.ttf)
endif()

juce_add_binary_data(Os251Binaries
    SOURCES
    ${BINARY_SOURCES}
)

target_sources(Os251 PRIVATE
        PluginProcessor.cpp
        PluginEditor.cpp
        dsp/Chorus.cpp
        dsp/Envelope.cpp
        synth/SynthEngine.cpp
        synth/SynthVoice.cpp
        services/PresetManager.cpp
        views/PresetManagerView.cpp
        views/ClippingIndicatorView.cpp
        )

# Add an explicit include path here so that react-juce's EcmascriptEngine can find
# the included Duktape source. You can optionally point this elsewhere if you'd like to
# include a custom Duktape build.
target_include_directories(Os251 PRIVATE react_juce/)

target_link_libraries(Os251 PUBLIC
        Os251Binaries
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
        react_juce
        )

# Avoid link error on linux + gcc
# https://forum.juce.com/t/loading-pytorch-model-using-binarydata/39997/2
set_target_properties(Os251Binaries PROPERTIES
        POSITION_INDEPENDENT_CODE TRUE
        )

juce_generate_juce_header(Os251)
