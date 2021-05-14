cmake_minimum_required(VERSION 3.15)

PROJECT(OS_251
        LANGUAGES CXX
        VERSION 0.1.3
        )

# for clang-tidy(this enable to find system header files).
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

#
# Add googletest and google/benchmark
# https://stackoverflow.com/questions/55376111/how-to-build-and-link-google-benchmark-using-cmake-in-windows
#

include(FetchContent)

# Externally provided libraries
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git)

FetchContent_Declare(googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.5.2)

FetchContent_MakeAvailable(
        googletest
        googlebenchmark)

# Enable JUCE. Do not use find_package to prevent from mix up with one globally installed.
add_subdirectory(lib/JUCE)

# Adding any custom modules you might have:
juce_add_module(lib/react-juce/react_juce)

add_subdirectory(src)
add_subdirectory(benchmark)