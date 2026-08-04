#pragma once

// Must be included BEFORE you declare any JUCE classes
#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include <juce_gui_basics/juce_gui_basics.h>

#include "DSP/DCBlocker.h"
#include "DSP/NoiseGate.h"
#include "DSP/Distortion.h"
#include "DSP/ToneStack.h"

class OthrysAudioProcessor : public juce::AudioProcessor
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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OthrysAudioProcessor)
};