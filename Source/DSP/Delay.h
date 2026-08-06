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
<<<<<<< HEAD
            auto& block = context.getOutputBlock(); // Processing in-place
            const int numChannels = static_cast<int> (block.getNumChannels());
            const int numSamples = static_cast<int> (block.getNumSamples());

            for (int channel = 0; channel < numChannels; ++channel)
            {
                auto* channelData = block.getChannelPointer (channel);

                for (int sample = 0; sample < numSamples; ++sample)
                {
                    const float inputSample = channelData[sample];

                    // Read the delayed sample from the buffer
                    const float delayedSample = delayLine.popSample (channel);

                    // Calculate the final output sample by mixing dry and wet
                    channelData[sample] = (inputSample * (1.0f - mix)) + (delayedSample * mix);

                    // Write the new value (input + feedback) into the delay line for the next iteration
                    delayLine.pushSample (channel, inputSample + (delayedSample * feedback));
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
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
