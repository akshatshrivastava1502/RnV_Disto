#pragma once
#include <juce_dsp/juce_dsp.h>
#include <vector>

namespace Othrys
{
namespace DSP
{
    class Delay
    {
    public:
        Delay() : delayLine (192000) // Support up to 192kHz sample rate for a 1-second delay (192000 samples)
        {
        }

        ~Delay() = default;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            sampleRate = spec.sampleRate;
            
            // Set up delay line
            delayLine.prepare (spec);
            
            reset();
        }

        void updateParameters (float mixPct, float timeMs, float feedbackPct)
        {
            mix = mixPct / 100.0f;
            
            // Calculate delay in samples
            float delaySamples = (timeMs / 1000.0f) * static_cast<float>(sampleRate);
            delayLine.setDelay (delaySamples);
            
            feedback = feedbackPct / 100.0f;
        }

        void process (const juce::dsp::ProcessContextReplacing<float>& context)
        {
            auto& inputBlock = context.getInputBlock();
            auto& outputBlock = context.getOutputBlock();
            
            const int numChannels = static_cast<int> (inputBlock.getNumChannels());
            const int numSamples = static_cast<int> (inputBlock.getNumSamples());

            for (int sample = 0; sample < numSamples; ++sample)
            {
                for (int channel = 0; channel < numChannels; ++channel)
                {
                    float inputSample = inputBlock.getSample (channel, sample);
                    
                    // Read delayed sample without advancing pointer
                    float delayedSample = delayLine.popSample (channel, -1.0f, false);
                    
                    // Write input + feedback into delay line
                    float inputToDelay = inputSample + (delayedSample * feedback);
                    
                    // Push into delay line
                    delayLine.pushSample (channel, inputToDelay);
                    
                    // Advance pointer and pop final wet sample
                    float finalWet = delayLine.popSample (channel, -1.0f, true);
                    
                    // Blend wet/dry
                    float outputSample = (inputSample * (1.0f - mix)) + (finalWet * mix);
                    outputBlock.setSample (channel, sample, outputSample);
                }
            }
        }

        void reset()
        {
            delayLine.reset();
        }

    private:
        juce::dsp::DelayLine<float> delayLine;
        double sampleRate = 44100.0;
        
        float mix = 0.0f;
        float feedback = 0.0f;
    };
}
}
