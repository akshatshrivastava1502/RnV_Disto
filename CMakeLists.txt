cmake_minimum_required(VERSION 3.21)

# Project Definition
project(Othrys VERSION 0.1.0 LANGUAGES CXX)

# Enforce Modern C++20 Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Fetch JUCE directly from GitHub (Industry Standard Approach)
include(FetchContent)
FetchContent_Declare(
    juce
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        8.0.0 # Using a stable, modern release
)
FetchContent_MakeAvailable(juce)

# Fetch ASIO SDK on Windows for low-latency audio support
if (WIN32)
    FetchContent_Declare(
        asio_sdk
        GIT_REPOSITORY https://github.com/audiosdk/asio.git
        GIT_TAG        main
    )
    FetchContent_MakeAvailable(asio_sdk)
endif()

# Define the JUCE Plugin Target
juce_add_plugin(Othrys
    COMPANY_NAME "ProdByRnV"
    PLUGIN_MANUFACTURER_CODE "RnVd" # Must be exactly 4 characters
    PLUGIN_CODE "Rnv1"              # Must be exactly 4 characters
    FORMATS VST3 AU Standalone      # Formats defined in PRD
    PRODUCT_NAME "Othrys"
)

juce_generate_juce_header(Othrys)

target_compile_definitions(Othrys
    PUBLIC
        JUCE_VST3_CAN_REPLACE_VST2=0
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
)

if (WIN32)
    target_compile_definitions(Othrys PRIVATE JUCE_ASIO=1)
    target_include_directories(Othrys PRIVATE "${asio_sdk_SOURCE_DIR}/common")
endif()

# Target our source files
target_sources(Othrys PRIVATE
    Source/PluginProcessor.h
    Source/PluginProcessor.cpp
    Source/PluginEditor.h
    Source/PluginEditor.cpp
    Source/DSP/Delay.h
    Source/DSP/Reverb.h
    Source/DSP/NoiseGate.h
)

# Link necessary JUCE modules (including juce_dsp for our core processing)
target_link_libraries(Othrys
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp           # <-- Add this crucial line
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)

# Enforce strict compiler warnings for commercial-guide safety
if (MSVC)
    target_compile_options(Othrys PRIVATE /W4 /WX)
else()
    target_compile_options(Othrys PRIVATE -Wall -Wextra -Werror)
endif()