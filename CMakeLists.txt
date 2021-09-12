cmake_minimum_required(VERSION 3.15)
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.12" CACHE STRING "Minimum OS X deployment target")

PROJECT(OS_251
        LANGUAGES CXX
        VERSION 1.0.1
        )


# for clang-tidy(this enable to find system header files).
if(APPLE AND CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()
#
# Add googletest and google/benchmark
# https://stackoverflow.com/questions/55376111/how-to-build-and-link-google-benchmark-using-cmake-in-windows
# https://cmake.org/cmake/help/latest/module/FetchContent.html
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


####################################################### -> react-juce
# Written based on lib/react-juce/CMakeLists.txt
# I think after https://github.com/nick-thompson/react-juce/pull/254 is merged, 
# we can use add_subdirectory(lib/react-juce) or CPM instead of copying
# react-juce's variables.

# Adding any custom modules you might have:
juce_add_module(lib/react-juce/react_juce)

# Change this option to set the JS Interpreter/Engine you wish to run React-JUCE against.
set(REACTJUCE_JS_LIBRARY HERMES CACHE STRING "The JS Engine to use: either HERMES or DUKTAPE")

# Setup the JS engine/interpreter to use.
if (REACTJUCE_JS_LIBRARY STREQUAL "HERMES")
    add_subdirectory(lib/react-juce/react_juce/hermes)


    target_compile_definitions(
        react_juce
        INTERFACE
        REACTJUCE_USE_HERMES=1
    )

    #TODO: We should be able to remove this include bloc once the following PR is merged
    #      over at hermes upstream: https://github.com/facebook/hermes/pull/454
    target_include_directories(
        react_juce
        INTERFACE
        lib/react-juce/react_juce/hermes/API/
        lib/react-juce/react_juce/hermes/public/
    )

    target_link_libraries(
        react_juce
        INTERFACE
        hermesapi
    )
elseif (REACTJUCE_JS_LIBRARY STREQUAL "DUKTAPE")
    target_compile_definitions(
        react_juce
        INTERFACE
        REACTJUCE_USE_DUKTAPE=1
    )
endif()

#######################################################<- react-juce

enable_testing()
configure_file(${CMAKE_SOURCE_DIR}/CTestCustom.cmake ${CMAKE_BINARY_DIR})

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(benchmark)
