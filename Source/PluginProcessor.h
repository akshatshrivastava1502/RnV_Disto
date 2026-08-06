#pragma once

<<<<<<< HEAD
#include <juce_audio_processors/juce_audio_processors.h>
#include <map>
#include <juce_dsp/juce_dsp.h>

// --- Your Custom Architecture Includes ---
#include "Parameters/ParameterDefines.h"
#include "DSP/DCBlocker.h"
#include "DSP/Delay.h"
#include "DSP/Distortion.h"
#include "DSP/Reverb.h"
#include "DSP/GraphicEQ.h"
#include "DSP/NoiseGate.h"
#include "DSP/ToneStack.h"
// -----------------------------------------

// ... rest of your code

class OthrysAudioProcessor  : public juce::AudioProcessor, private juce::Timer
=======
// Must be included BEFORE you declare any JUCE classes
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "DSP/DCBlocker.h"
#include "DSP/NoiseGate.h"
#include "DSP/Distortion.h"
#include "DSP/ToneStack.h"

class OthrysAudioProcessor : public juce::AudioProcessor
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
{
public:
    OthrysAudioProcessor();
    ~OthrysAudioProcessor() override;

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

<<<<<<< HEAD
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
    Othrys::DSP::DCBlocker dcBlocker;
    Othrys::DSP::Distortion distortion;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;
    Othrys::DSP::Delay delay;
    Othrys::DSP::Reverb reverb;
    Othrys::DSP::GraphicEQ graphicEQ;
    Othrys::DSP::ToneStack toneStack; // Re-added ToneStack
    Othrys::DSP::NoiseGate noiseGate;

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
    std::atomic<float>* eqPowerParam = nullptr;

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

    // EQ Parameters
    std::atomic<float>* eqHpfFreqParam = nullptr;
    std::atomic<float>* eqLpfFreqParam = nullptr;
    std::atomic<float>* eqBand65HzParam   = nullptr;
    std::atomic<float>* eqBand125HzParam  = nullptr;
    std::atomic<float>* eqBand250HzParam  = nullptr;
    std::atomic<float>* eqBand500HzParam  = nullptr;
    std::atomic<float>* eqBand1kHzParam   = nullptr;
    std::atomic<float>* eqBand2kHzParam   = nullptr;
    std::atomic<float>* eqBand4kHzParam   = nullptr;
    std::atomic<float>* eqBand8kHzParam   = nullptr;
    std::atomic<float>* eqBand16kHzParam  = nullptr;

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
=======
    // --- PRESET MANAGEMENT ---
    // Moved to PUBLIC scope so PluginEditor can access them
    void savePresetToFile(const juce::String& presetName);
    void loadPresetFromFile(const juce::File& presetFile);
    juce::File getPresetsFolder();
    juce::Array<juce::File> getPresetFiles();

    // --- APVTS ---
    // Declared here to fix the "undeclared identifier" error in the .cpp
    juce::AudioProcessorValueTreeState apvts;

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

    // --- DSP MODULES ---
    Othrys::DSP::DCBlocker dcBlocker;
    Othrys::DSP::NoiseGate noiseGate;
    Othrys::DSP::Distortion distortion;
    Othrys::DSP::ToneStack toneStack;
    juce::dsp::DryWetMixer<float> dryWetMixer;

    // --- OVERSAMPLING ---
    // Instantiated directly to avoid missing initialization list errors. 
    // Arguments: 2 Channels, Factor 1 (2x Oversampling), IIR Filter, Phase Compensated = true
    juce::dsp::Oversampling<float> oversampler { 2, 1, juce::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR, true };

    // --- ATOMIC POINTERS ---
    // Used to safely read parameter values on the audio thread without locking
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* hpfCutoffParam = nullptr;
    std::atomic<float>* lpfCutoffParam = nullptr;
    std::atomic<float>* outputGainParam = nullptr;
    std::atomic<float>* dryWetParam = nullptr;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OthrysAudioProcessor)
};