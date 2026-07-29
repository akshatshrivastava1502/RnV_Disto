#pragma once
#include <vector>

namespace RnVDisto
{
namespace DSP
{

class DCBlocker
{
public:
    DCBlocker() = default;

    // Called in prepareToPlay to set up our channels and sample rate
    void prepare (double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::ignoreUnused (sampleRate, samplesPerBlock);

        // Resize our state vectors to match the number of audio channels (e.g., 2 for stereo)
        x1.resize (numChannels, 0.0f);
        y1.resize (numChannels, 0.0f);

        reset();
    }

    // Clears the filter state to prevent ringing or pops when playback starts
    void reset()
    {
        std::fill (x1.begin(), x1.end(), 0.0f);
        std::fill (y1.begin(), y1.end(), 0.0f);
    }

    // The core DSP processing function (Real-time safe)
    void process (juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples  = buffer.getNumSamples();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);

            for (int sample = 0; sample < numSamples; ++sample)
            {
                const float x = channelData[sample];

                // y[n] = x[n] - x[n-1] + R * y[n-1]
                const float y = x - x1[channel] + R * y1[channel];

                // Update state variables for the next iteration
                x1[channel] = x;
                y1[channel] = y;

                // Output the processed sample
                channelData[sample] = y;
            }
        }
    }

private:
    // Filter coefficient (Pole). 0.995 provides a cutoff around ~10-20Hz depending on sample rate.
    const float R = 0.995f;

    // State variables for previous input (x1) and previous output (y1) per channel
    std::vector<float> x1;
    std::vector<float> y1;
};

} // namespace DSP
} // namespace RnVDisto