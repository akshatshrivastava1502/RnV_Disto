#pragma once
#include <juce_audio_processors/juce_audio_processors.h>

namespace Othrys
{
namespace DSP
{

enum class DistortionType
{
    Soft,
    Hard,
    Foldback,
    Tube,
    Tape // New Algorithm Added
};

class Distortion
{
public:
    Distortion() = default;

    void setType (DistortionType newType)
    {
        type = newType;
    }

    void setDrive (float newDriveDb)
    {
        driveLinear = juce::Decibels::decibelsToGain (newDriveDb);
    }

    void process (juce::dsp::AudioBlock<float>& block)
    {
        const int numChannels = (int) block.getNumChannels();
        const int numSamples  = (int) block.getNumSamples();

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = block.getChannelPointer (channel);

            for (int sample = 0; sample < numSamples; ++sample)
            {
                float x = channelData[sample] * driveLinear;
                
                // Routing based on user selection
                if (type == DistortionType::Hard)
                    channelData[sample] = hardClip (x);
                else if (type == DistortionType::Foldback)
                    channelData[sample] = foldback (x);
                else if (type == DistortionType::Tube)
                    channelData[sample] = tubeClip (x);
                else if (type == DistortionType::Tape)
                    channelData[sample] = tapeClip (x);
                else
                    channelData[sample] = softClip (x);
            }
        }
    }

private:
    DistortionType type { DistortionType::Soft };
    float driveLinear { 1.0f };

    // --- Nonlinear Algorithms --- //

    inline float hardClip (float x)
    {
        return juce::jlimit (-1.0f, 1.0f, x);
    }

    inline float softClip (float x)
    {
        return x / (1.0f + std::abs (x));
    }

    inline float foldback (float x)
    {
        return std::sin (x * juce::MathConstants<float>::halfPi);
    }

    inline float tubeClip (float x)
    {
        if (x > 0.0f)
            return x / (1.0f + x); 
        else
            return x / (1.0f + 0.25f * std::abs (x)); 
    }

    // New: Magnetic Tape Modeling (Cubic Saturation)
    inline float tapeClip (float x)
    {
        // If the signal is within the soft saturation boundaries
        if (x > -1.0f && x < 1.0f)
        {
            // y = 1.5 * (x - (x^3 / 3))
            return 1.5f * (x - ((x * x * x) / 3.0f));
        }
        // If pushed beyond the boundaries, it hits absolute magnetic saturation
        else
        {
            return x >= 1.0f ? 1.0f : -1.0f;
        }
    }
};

} // namespace DSP
} // namespace Othrys