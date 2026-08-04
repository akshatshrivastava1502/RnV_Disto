#pragma once
#include <vector>
#include <juce_dsp/juce_dsp.h>

namespace Othrys
{
namespace DSP
{

class DCBlocker
{
public:
    DCBlocker() = default;

    void prepare (double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::ignoreUnused (sampleRate, samplesPerBlock);
        
        // Resize our state vectors to match the number of audio channels (e.g., 2 for stereo)
        x1.resize (numChannels, 0.0f);
        y1.resize (numChannels, 0.0f);

        reset();
    }

    void reset()
    {
        std::fill (x1.begin(), x1.end(), 0.0f);
        std::fill (y1.begin(), y1.end(), 0.0f);
    }

    void process (juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples  = buffer.getNumSamples();

        // Real-time safety check against out-of-bounds memory access
        if (x1.size() < static_cast<size_t>(numChannels)) return;

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);

            // 1. Load state into local registers BEFORE the inner loop
            float currentX1 = x1[channel];
            float currentY1 = y1[channel];

            for (int sample = 0; sample < numSamples; ++sample)
            {
                const float x = channelData[sample];

                // 2. Compute using fast local variables
                // y[n] = x[n] - x[n-1] + R * y[n-1]
                const float y = x - currentX1 + R * currentY1;

                // 3. Update local state
                currentX1 = x;
                currentY1 = y;

                // Output the processed sample
                channelData[sample] = y;
            }

            // 4. Save state back to the vector AFTER the loop
            x1[channel] = currentX1;
            y1[channel] = currentY1;
        }
    }

private:
    const float R = 0.995f;
    std::vector<float> x1;
    std::vector<float> y1;
};

} // namespace DSP
} // namespace Othrys