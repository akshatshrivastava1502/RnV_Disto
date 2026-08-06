#pragma once
#include <vector>
<<<<<<< HEAD
=======
#include <juce_dsp/juce_dsp.h>
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1

namespace Othrys
{
namespace DSP
{

class DCBlocker
{
public:
    DCBlocker() = default;

<<<<<<< HEAD
    // Called in prepareToPlay to set up our channels and sample rate
    void prepare (double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::ignoreUnused (sampleRate, samplesPerBlock);

=======
    void prepare (double sampleRate, int samplesPerBlock, int numChannels)
    {
        juce::ignoreUnused (sampleRate, samplesPerBlock);
        
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
        // Resize our state vectors to match the number of audio channels (e.g., 2 for stereo)
        x1.resize (numChannels, 0.0f);
        y1.resize (numChannels, 0.0f);

        reset();
    }

<<<<<<< HEAD
    // Clears the filter state to prevent ringing or pops when playback starts
=======
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    void reset()
    {
        std::fill (x1.begin(), x1.end(), 0.0f);
        std::fill (y1.begin(), y1.end(), 0.0f);
    }

<<<<<<< HEAD
    // The core DSP processing function (Real-time safe)
=======
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    void process (juce::AudioBuffer<float>& buffer)
    {
        const int numChannels = buffer.getNumChannels();
        const int numSamples  = buffer.getNumSamples();

<<<<<<< HEAD
=======
        // Real-time safety check against out-of-bounds memory access
        if (x1.size() < static_cast<size_t>(numChannels)) return;

>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer (channel);

<<<<<<< HEAD
=======
            // 1. Load state into local registers BEFORE the inner loop
            float currentX1 = x1[channel];
            float currentY1 = y1[channel];

>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
            for (int sample = 0; sample < numSamples; ++sample)
            {
                const float x = channelData[sample];

<<<<<<< HEAD
                // y[n] = x[n] - x[n-1] + R * y[n-1]
                const float y = x - x1[channel] + R * y1[channel];

                // Update state variables for the next iteration
                x1[channel] = x;
                y1[channel] = y;
=======
                // 2. Compute using fast local variables
                // y[n] = x[n] - x[n-1] + R * y[n-1]
                const float y = x - currentX1 + R * currentY1;

                // 3. Update local state
                currentX1 = x;
                currentY1 = y;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1

                // Output the processed sample
                channelData[sample] = y;
            }
<<<<<<< HEAD
=======

            // 4. Save state back to the vector AFTER the loop
            x1[channel] = currentX1;
            y1[channel] = currentY1;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
        }
    }

private:
<<<<<<< HEAD
    // Filter coefficient (Pole). 0.995 provides a cutoff around ~10-20Hz depending on sample rate.
    const float R = 0.995f;

    // State variables for previous input (x1) and previous output (y1) per channel
=======
    const float R = 0.995f;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    std::vector<float> x1;
    std::vector<float> y1;
};

} // namespace DSP
} // namespace Othrys