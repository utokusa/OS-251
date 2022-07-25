cmake_minimum_required(VERSION 3.20)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")

# It cause fe_expected_compiler_job locally.
# So create universal binaries only on CI/CD. 
# set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE STRING "Build as universal binary")

PROJECT(OS_251
        LANGUAGES CXX C
        VERSION 1.2.1
        )

add_compile_definitions(OS_251_PROJECT_VERSION="${PROJECT_VERSION}")


# for clang-tidy(this enable to find system header files).
if(APPLE AND CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

#
# Add googletest and google/benchmark
# https://stackoverflow.com/questions/55376111/how-to-build-and-link-google-benchmark-using-cmake-in-windows
#

include(FetchContent)

# Externally provided libraries
FetchContent_Declare(googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
        )

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

enable_testing()
configure_file(${CMAKE_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmark)
