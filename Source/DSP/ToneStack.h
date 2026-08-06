#pragma once
#include <juce_dsp/juce_dsp.h>
<<<<<<< HEAD

namespace Othrys
{
namespace DSP
{

    class ToneStack
    {
    public:
        ToneStack() = default;
        ~ToneStack() = default;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            sampleRate = spec.sampleRate;
            lowShelf.prepare (spec);
            highShelf.prepare (spec);

            // 50ms smoothing time to prevent zipper noise from automation
            smoothedBassDb.reset (spec.sampleRate, 0.05);
            smoothedTrebleDb.reset (spec.sampleRate, 0.05);
            
            // Initialize with flat EQ (0 dB)
            updateCoefficients(); 
        }

        void setTone (float toneValue) // toneValue expected from 0.0 (dark) to 1.0 (bright)
        {
            // Simple tilt-style mapping: 
            // At 0.0 -> Bass boosted (+3dB), Treble cut (-3dB)
            // At 0.5 -> Flat (0dB, 0dB)
            // At 1.0 -> Bass cut (-3dB), Treble boosted (+3dB)
            smoothedBassDb.setTargetValue ((0.5f - toneValue) * 6.0f);
            smoothedTrebleDb.setTargetValue ((toneValue - 0.5f) * 6.0f);
        }

        // Call this from PluginProcessor::processBlock before processing audio
        void setParameters (float bassGainDb, float trebleGainDb)
        {
            smoothedBassDb.setTargetValue (bassGainDb);
            smoothedTrebleDb.setTargetValue (trebleGainDb);
        }

        void process (const juce::dsp::ProcessContextReplacing<float>& context)
        {
            updateCoefficients(); // Update coefficients smoothly at the start of each block
            lowShelf.process (context);
            highShelf.process (context);
        }

        void reset()
        {
            lowShelf.reset();
            highShelf.reset();
        }

    private:
        void updateCoefficients()
        {
            if (sampleRate > 0.0)
            {
                bassDb = smoothedBassDb.getNextValue();
                trebleDb = smoothedTrebleDb.getNextValue();

                // We use * to copy the float array directly into the state, 
                // avoiding memory allocations on the audio thread.
                *lowShelf.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf (sampleRate, 250.0f, 0.707f, juce::Decibels::decibelsToGain (bassDb));
                *highShelf.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf (sampleRate, 2500.0f, 0.707f, juce::Decibels::decibelsToGain (trebleDb));
            }
        }

        // The ProcessorDuplicator automatically turns a mono IIR filter into a stereo one.
        using FilterType = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;
        
        FilterType lowShelf;
        FilterType highShelf;

        double sampleRate = 44100.0;
        float bassDb = 0.0f;
        float trebleDb = 0.0f;

        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedBassDb;
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedTrebleDb;
    };
=======
#include <algorithm>
#include <cmath>

namespace Othrys {
namespace DSP {

class ToneStack {
public:
    ToneStack() {
        // Correctly referencing the strict JUCE Enum types
        tiltFilter.setType(juce::dsp::FirstOrderTPTFilter<float>::Type::lowpass);
        seriesHpf.setType(juce::dsp::StateVariableTPTFilter<float>::Type::highpass);
        seriesLpf.setType(juce::dsp::StateVariableTPTFilter<float>::Type::lowpass);
    }

    void prepare(const juce::dsp::ProcessSpec& spec) {
        tiltFilter.prepare(spec);
        seriesHpf.prepare(spec);
        seriesLpf.prepare(spec);
        
        // Set the pivot frequency for our Tilt EQ
        tiltFilter.setCutoffFrequency(1000.0f);
    }

    void setFilters(float hpfFreq, float lpfFreq) {
        seriesHpf.setCutoffFrequency(hpfFreq);
        seriesLpf.setCutoffFrequency(lpfFreq);
    }

    void setTone(float rawToneParam) {
        // Assuming rawToneParam is mapped 0.0 to 1.0 in your APVTS
        if (std::abs(rawToneParam - lastTone) < 0.001f) return;
        lastTone = rawToneParam;

        // Convert 0.0 - 1.0 knob range into a bipolar -1.0 to 1.0 tilt
        float tilt = (rawToneParam * 2.0f) - 1.0f; 
        
        // Calculate crossover mixing gains
        // Tilt > 0: Boost highs, cut lows
        // Tilt < 0: Boost lows, cut highs
        lowGain = 1.0f - std::max(0.0f, tilt);
        highGain = 1.0f - std::max(0.0f, -tilt);
    }

    void process(juce::dsp::AudioBlock<float>& block) {
        // 1. Phase 4 Series High-Pass Filter
        seriesHpf.process(juce::dsp::ProcessContextReplacing<float>(block));
        
        // 2. Zero-Allocation Tilt EQ
        auto numChannels = block.getNumChannels();
        auto numSamples = block.getNumSamples();

        for (size_t ch = 0; ch < numChannels; ++ch) {
            auto* channelData = block.getChannelPointer(ch);
            for (size_t i = 0; i < numSamples; ++i) {
                float input = channelData[i];
                
                // Extract low frequencies
                float lowFreq = tiltFilter.processSample(static_cast<int>(ch), input);
                
                // Extract high frequencies via phase cancellation
                float highFreq = input - lowFreq;
                
                // Recombine with tilt gains
                channelData[i] = (lowFreq * lowGain) + (highFreq * highGain);
            }
        }
        
        // 3. Phase 4 Series Low-Pass Filter
        seriesLpf.process(juce::dsp::ProcessContextReplacing<float>(block));
    }

private:
    juce::dsp::FirstOrderTPTFilter<float> tiltFilter;
    juce::dsp::StateVariableTPTFilter<float> seriesHpf;
    juce::dsp::StateVariableTPTFilter<float> seriesLpf;
    
    float lastTone = -999.0f;
    float lowGain = 1.0f;
    float highGain = 1.0f;
};
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1

} // namespace DSP
} // namespace Othrys