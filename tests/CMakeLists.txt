juce_add_console_app(Os251_Tests)

target_compile_features(Os251_Tests PUBLIC cxx_std_17)

target_link_libraries(Os251_Tests PUBLIC
        gtest_main)

target_compile_definitions(Os251_Tests
        PUBLIC
        # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
        JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
        JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
        )

target_sources(Os251_Tests PRIVATE
        EnvelopeTest.cpp
        OscillatorTest.cpp
        ../src/Envelope.cpp
        # Uncomment below if you want to run JuceTestExample.cpp
        # JuceTestExample.cpp
        )

target_link_libraries(Os251_Tests PUBLIC
        # Uncomment below if you want to run JuceTestExample.cpp
        # juce::juce_core
        )

juce_generate_juce_header(Os251_Tests)

include(GoogleTest)
gtest_discover_tests(Os251_Tests)
