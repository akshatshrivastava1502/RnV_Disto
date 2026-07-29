#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

// --- Your Custom Architecture Includes ---
#include "Parameters/ParameterDefines.h"
#include "DSP/DCBlocker.h"
#include "DSP/Distortion.h" 
#include "DSP/ToneStack.h"    // <--- Add this missing link
// -----------------------------------------

// ... rest of your code

class RnVDistoAudioProcessor  : public juce::AudioProcessor
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

    juce::AudioProcessorValueTreeState apvts;

private:
    // --- DSP Modules ---
    juce::dsp::Gain<float> inputGain;
    RnVDisto::DSP::DCBlocker dcBlocker;
    RnVDisto::DSP::Distortion distortion;
    RnVDisto::DSP::ToneStack toneStack;
    std::unique_ptr<juce::dsp::Oversampling<float>> oversampler;

    // NEW: Output Stage Modules
    juce::dsp::Gain<float> outputGain;
    juce::dsp::Limiter<float> limiter;
    juce::dsp::DryWetMixer<float> dryWetMixer;

    // --- Cached Parameter Pointers (For Lock-Free Real-Time Reads) ---
    std::atomic<float>* inputGainParam = nullptr;
    std::atomic<float>* driveParam = nullptr;
    std::atomic<float>* typeParam = nullptr;
    std::atomic<float>* toneParam = nullptr;
    std::atomic<float>* outputGainParam = nullptr;
    std::atomic<float>* mixParam = nullptr;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RnVDistoAudioProcessor)
};