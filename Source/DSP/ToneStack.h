#pragma once
#include <juce_dsp/juce_dsp.h>
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

} // namespace DSP
} // namespace Othrys