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
    const juce::String typeID       = "type";

    // Reverb IDs
    const juce::String reverbTypeID  = "reverb_type";
    const juce::String reverbMixID   = "reverb_mix";
    const juce::String reverbSizeID  = "reverb_size";
    const juce::String reverbDampID  = "reverb_damp";
    const juce::String reverbWidthID = "reverb_width";

    // Delay IDs
    const juce::String delayMixID      = "delay_mix";
    const juce::String delayTimeID     = "delay_time";
    const juce::String delayFeedbackID = "delay_feedback";

    // Noise Gate IDs
    const juce::String gateThresholdID = "gate_threshold";
    const juce::String gateAttackID    = "gate_attack";
    const juce::String gateReleaseID   = "gate_release";

    // Human-readable names
    const juce::String inputGainName  = "Input Gain";
    const juce::String driveName      = "Drive";
    const juce::String outputGainName = "Output Gain";
    const juce::String mixName        = "Mix";
    const juce::String typeName       = "Distortion Type";

    inline juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout()
    {
        std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

        // Distortion / Gain parameters
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (inputGainID, 1), inputGainName,
            juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (driveID, 1), driveName,
            juce::NormalisableRange<float> (0.0f, 100.0f, 0.1f), 0.0f));

        juce::StringArray distortionTypes = { "Soft Clip", "Hard Clip", "Foldback", "Tube", "Tape" };
        params.push_back (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID (typeID, 1), typeName, distortionTypes, 0));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID ("tone", 1), "Tone",
            juce::NormalisableRange<float> (-1.0f, 1.0f, 0.01f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (outputGainID, 1), outputGainName,
            juce::NormalisableRange<float> (-24.0f, 24.0f, 0.1f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (mixID, 1), mixName,
            juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 100.0f));

        // Reverb parameters
        juce::StringArray reverbTypes = { "Room", "Hall" };
        params.push_back (std::make_unique<juce::AudioParameterChoice> (
            juce::ParameterID (reverbTypeID, 1), "Reverb Type", reverbTypes, 0));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (reverbMixID, 1), "Reverb Mix",
            juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (reverbSizeID, 1), "Reverb Size",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (reverbDampID, 1), "Reverb Damping",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.3f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (reverbWidthID, 1), "Reverb Width",
            juce::NormalisableRange<float> (0.0f, 1.0f, 0.01f), 0.5f));

        // Delay parameters
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (delayMixID, 1), "Delay Mix",
            juce::NormalisableRange<float> (0.0f, 100.0f, 1.0f), 0.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (delayTimeID, 1), "Delay Time",
            juce::NormalisableRange<float> (1.0f, 2000.0f, 1.0f), 300.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (delayFeedbackID, 1), "Delay Feedback",
            juce::NormalisableRange<float> (0.0f, 95.0f, 1.0f), 30.0f));

        // Noise Gate parameters
        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (gateThresholdID, 1), "Gate Threshold",
            juce::NormalisableRange<float> (-100.0f, 0.0f, 0.1f), -80.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (gateAttackID, 1), "Gate Attack",
            juce::NormalisableRange<float> (0.1f, 100.0f, 0.1f), 1.0f));

        params.push_back (std::make_unique<juce::AudioParameterFloat> (
            juce::ParameterID (gateReleaseID, 1), "Gate Release",
            juce::NormalisableRange<float> (10.0f, 1000.0f, 1.0f), 100.0f));

        return { params.begin(), params.end() };
    }
} // namespace Parameters
} // namespace RnVDisto