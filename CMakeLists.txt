cmake_minimum_required(VERSION 3.22)
project(FLCamera VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.4
    GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(JUCE)

# Na nuvem (GitHub Actions) não copiamos o plugin para a pasta do sistema.
if(DEFINED ENV{CI})
    set(FLCAM_COPY FALSE)
else()
    set(FLCAM_COPY TRUE)
endif()

juce_add_plugin(FLCamera
    COMPANY_NAME "FLCamera"
    PLUGIN_MANUFACTURER_CODE Flcm
    PLUGIN_CODE Fcam
    FORMATS VST3
    PRODUCT_NAME "FL Camera"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    VST3_CATEGORIES Fx
    COPY_PLUGIN_AFTER_BUILD ${FLCAM_COPY})

target_sources(FLCamera PRIVATE
    Source/PluginProcessor.cpp
    Source/PluginProcessor.h
    Source/PluginEditor.cpp
    Source/PluginEditor.h)

target_compile_definitions(FLCamera PUBLIC
    JUCE_WEB_BROWSER=0
    JUCE_USE_CURL=0
    JUCE_VST3_CAN_REPLACE_VST2=0
    JUCE_USE_CAMERA=1)

target_link_libraries(FLCamera
    PRIVATE
        juce::juce_audio_utils
        juce::juce_video
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_warning_flags)
