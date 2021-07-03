cmake_minimum_required(VERSION 3.14)

add_executable(Os251_Tests)

target_compile_features(Os251_Tests PUBLIC cxx_std_17)

target_link_libraries(Os251_Tests PUBLIC
        gtest_main)

target_sources(Os251_Tests PRIVATE
        ChorusTest.cpp
        EnvelopeTest.cpp
        OscillatorTest.cpp
        LfoTest.cpp
        FilterTest.cpp
        HpfTest.cpp
        MasterVolumeTest.cpp
        TestAudioBufferInput.cpp
        ../src/Chorus.cpp
        ../src/Envelope.cpp
        )

include(GoogleTest)
gtest_discover_tests(Os251_Tests)
