#pragma once
#include <juce_dsp/juce_dsp.h>
#include <cmath>
#include <vector>

namespace RnVDisto
{
namespace DSP
{
    class NoiseGate
    {
    public:
        NoiseGate() = default;
        ~NoiseGate() = default;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            sampleRate = spec.sampleRate;
            envelope.resize (spec.numChannels, 0.0f);
            currentGain.resize (spec.numChannels, 1.0f);
            reset();
        }

        void updateParameters (float thresholdDb, float attackMs, float releaseMs)
        {
            threshold = juce::Decibels::decibelsToGain (thresholdDb);
            
            // Calculate coefficients for envelope follower & gain smoothing
            // Prevent division by zero or negative values
            float attackSecs = std::max (0.0001f, attackMs * 0.001f);
            float releaseSecs = std::max (0.0001f, releaseMs * 0.001f);
            
            attackCoef = std::exp (-1.0f / (attackSecs * static_cast<float> (sampleRate)));
            releaseCoef = std::exp (-1.0f / (releaseSecs * static_cast<float> (sampleRate)));
        }

        void process (const juce::dsp::ProcessContextReplacing<float>& context)
        {
            auto& inputBlock = context.getInputBlock();
            auto& outputBlock = context.getOutputBlock();
            
            const int numChannels = static_cast<int> (inputBlock.getNumChannels());
            const int numSamples = static_cast<int> (inputBlock.getNumSamples());
            
            if (envelope.size() < static_cast<size_t> (numChannels))
            {
                envelope.resize (numChannels, 0.0f);
                currentGain.resize (numChannels, 1.0f);
            }

            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* inData = inputBlock.getChannelPointer (channel);
                auto* outData = outputBlock.getChannelPointer (channel);
                
                float env = envelope[static_cast<size_t> (channel)];
                float gain = currentGain[static_cast<size_t> (channel)];

                for (int sample = 0; sample < numSamples; ++sample)
                {
                    float inputSample = inData[sample];
                    float absSample = std::abs (inputSample);
                    
                    // Envelope Follower
                    float envCoef = (absSample > env) ? attackCoef : releaseCoef;
                    env = absSample + envCoef * (env - absSample);
                    
                    // Gate decision
                    float targetGain = (env > threshold) ? 1.0f : 0.0f;
                    
                    // Smooth gain reduction coefficient
                    float gainCoef = (targetGain > gain) ? attackCoef : releaseCoef;
                    gain = targetGain + gainCoef * (gain - targetGain);
                    
                    outData[sample] = inputSample * gain;
                }
                
                envelope[static_cast<size_t> (channel)] = env;
                currentGain[static_cast<size_t> (channel)] = gain;
            }
        }

        void reset()
        {
            std::fill (envelope.begin(), envelope.end(), 0.0f);
            std::fill (currentGain.begin(), currentGain.end(), 1.0f);
        }

    private:
        double sampleRate = 44100.0;
        float threshold = 0.0f;
        float attackCoef = 0.0f;
        float releaseCoef = 0.0f;
        
        std::vector<float> envelope;
        std::vector<float> currentGain;
    };
}
}
