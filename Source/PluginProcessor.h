#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <map>
#include <juce_dsp/juce_dsp.h>

// --- Your Custom Architecture Includes ---
#include "Parameters/ParameterDefines.h"
#include "DSP/DCBlocker.h"
#include "DSP/Distortion.h" 
#include "DSP/ToneStack.h"    // <--- Add this missing link
#include "DSP/Delay.h"
#include "DSP/Reverb.h"
#include "DSP/NoiseGate.h"
// -----------------------------------------

// ... rest of your code

class RnVDistoAudioProcessor  : public juce::AudioProcessor, private juce::Timer
{
public:
    RnVDistoAudioProcessor();
    ~RnVDistoAudioProcessor() override;

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // Preset save/load helpers
    void savePresetToFile (const juce::File& file);
    void loadPresetFromFile (const juce::File& file);
    juce::File getPresetsFolder();
    juce::Array<juce::File> getPresetFiles();

    juce::AudioProcessorValueTreeState apvts;

private:
    juce::String getNeuralDspValue (const juce::MemoryBlock& data, const juce::String& key);

    // --- DSP Modules ---
    juce::dsp::Gain<float> inputGain;
    RnVDisto::DSP::DCBlocker dcBlocker;
    RnVDisto::DSP::Distortion distortion;
    RnVDisto::DSP::ToneStack toneStack;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    RnVDisto::DSP::Delay delay;
    RnVDisto::DSP::Reverb reverb;
    RnVDisto::DSP::NoiseGate noiseGate;

    // NEW: Output Stage Modules
    juce::dsp::Gain<float> outputGain;
    juce::dsp::Limiter<float> limiter;
    juce::dsp::DryWetMixer<float> dryWetMixer;

    // Power parameters
    std::atomic<float>* bypassParam = nullptr;
    std::atomic<float>* distortionPowerParam = nullptr;
    std::atomic<float>* reverbPowerParam = nullptr;
    std::atomic<float>* delayPowerParam = nullptr;
    std::atomic<float>* gatePowerParam = nullptr;

    // Channel parameters
    std::atomic<float>* inputChannelModeParam = nullptr;

    // --- Cached Parameter Pointers (For Lock-Free Real-Time Reads) ---
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* typeParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* outputGainParam = nullptr;
    std::atomic<float>* mixParam = nullptr;

    // Reverb parameters
    std::atomic<float>* reverbTypeParam = nullptr;
    std::atomic<float>* reverbMixParam = nullptr;
    std::atomic<float>* reverbSizeParam = nullptr;
    std::atomic<float>* reverbDampParam = nullptr;
    std::atomic<float>* reverbWidthParam = nullptr;

    // Delay parameters
    std::atomic<float>* delayMixParam = nullptr;
    std::atomic<float>* delayTimeParam = nullptr;
    std::atomic<float>* delayFeedbackParam = nullptr;

    // Noise Gate parameters
    std::atomic<float>* gateThresholdParam = nullptr;
    std::atomic<float>* gateAttackParam = nullptr;
    std::atomic<float>* gateReleaseParam = nullptr;

    // --- Parameter Transitions / Animations ---
    struct ParameterTransition
    {
        juce::RangedAudioParameter* parameter = nullptr;
        float startValue = 0.0f;
        float targetValue = 0.0f;
    };
    
    std::vector<ParameterTransition> activeTransitions;
    int transitionStep = 0;
    const int transitionDurationSteps = 25; // 25 steps * 16ms = ~400ms transition
    
    void startParameterTransition (const std::map<juce::String, float>& targetValues);
    void timerCallback() override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RnVDistoAudioProcessor)
};