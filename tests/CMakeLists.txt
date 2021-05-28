enable_testing()

juce_add_console_app(Os251_Tests)

target_compile_features(Os251_Tests PUBLIC cxx_std_17)

target_link_libraries(Os251_Tests PUBLIC
        gtest_main)

target_sources(Os251_Tests PRIVATE
        Main.cpp
        )

target_link_libraries(Os251_Tests PUBLIC
        juce::juce_core
        )

juce_generate_juce_header(Os251_Tests)

include(GoogleTest)
gtest_discover_tests(Os251_Tests)