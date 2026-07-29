#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace RnVDisto
{
namespace Parameters
{
    // String identifiers
    const juce::String inputGainID  = "input_gain";
    const juce::String driveID      = "drive";
    const juce::String outputGainID = "output_gain";
    const juce::String mixID        = "mix";
    const juce::String typeID       = "type"; // New Parameter ID

    // Human-readable names
    const juce::String inputGainName  = "Input Gain";
    const juce::String driveName      = "Drive";
    const juce::String outputGainName = "Output Gain";
    const juce::String mixName        = "Mix";
    const juce::String typeName       = "Distortion Type";

    inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (inputGainID, 1), inputGainName,
            juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (driveID, 1), driveName,
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f), 0.0f));

        juce::StringArray distortionTypes = { "Soft Clip", "Hard Clip", "Foldback", "Tube", "Tape" };
        params.push_back (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID (typeID, 1), typeName, distortionTypes, 0));

        // NEW: Tone Parameter (-1.0 to 1.0, Default: 0.0)
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("tone", 1), "Tone",
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (outputGainID, 1), outputGainName,
            juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (mixID, 1), mixName,
            juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 100.0f));

        return { params.begin(), params.end() };
    }
} // namespace Parameters
} // namespace RnVDisto