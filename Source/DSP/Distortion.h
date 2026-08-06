#pragma once
#include <juce_audio_processors/juce_audio_processors.h>
<<<<<<< HEAD
=======
#include <cmath>
#include <algorithm>
#include <vector>
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1

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
<<<<<<< HEAD
    Tape // New Algorithm Added
=======
    Tape,
    Transistor, 
    Custom      
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
};

class Distortion
{
public:
    Distortion() = default;

<<<<<<< HEAD
=======
    void prepare (const juce::dsp::ProcessSpec& spec)
    {
        sampleRate = spec.sampleRate;
        // Allocate envelope state memory for each channel
        envelope.resize (spec.numChannels, 0.0f);
        reset();
    }

    void reset()
    {
        std::fill (envelope.begin(), envelope.end(), 0.0f);
    }

>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    void setType (DistortionType newType)
    {
        type = newType;
    }

    void setDrive (float newDriveDb)
    {
        driveLinear = juce::Decibels::decibelsToGain (newDriveDb);
    }

<<<<<<< HEAD
=======
    void setDynamics (float depthPct, float attackMs, float releaseMs)
    {
        // depthPct is -100 to 100. Convert to a multiplier ratio (-1.0 to 1.0)
        dynamicDepth = depthPct / 100.0f;
        
        // Safety bounds
        float attackSecs = std::max (0.0001f, attackMs * 0.001f);
        float releaseSecs = std::max (0.0001f, releaseMs * 0.001f);
        
        // Calculate analog-style IIR smoothing coefficients
        attackCoef = static_cast<float>(std::exp (-1.0f / (attackSecs * sampleRate)));
        releaseCoef = static_cast<float>(std::exp (-1.0f / (releaseSecs * sampleRate)));
    }

>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    void process (juce::dsp::AudioBlock<float>& block)
    {
        const int numChannels = (int) block.getNumChannels();
        const int numSamples  = (int) block.getNumSamples();

<<<<<<< HEAD
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
=======
        // Safety check for envelope vector size
        if (envelope.size() < static_cast<size_t>(numChannels)) return;

        for (int channel = 0; channel < numChannels; ++channel)
        {
            float* channelData = block.getChannelPointer (channel);
            float currentEnv = envelope[channel];

            for (int sample = 0; sample < numSamples; ++sample)
            {
                float x = channelData[sample];
                
                // 1. Envelope Follower calculation (Read transient energy)
                float absX = std::abs (x);
                if (absX > currentEnv)
                    currentEnv = absX + attackCoef * (currentEnv - absX);
                else
                    currentEnv = absX + releaseCoef * (currentEnv - absX);

                // 2. Modulate Drive dynamically
                // If depth is positive, transient peaks increase drive. 
                // If depth is negative, transient peaks duck the drive.
                float modulatedDrive = driveLinear * (1.0f + (currentEnv * dynamicDepth));
                
                // Prevent negative drive inversion
                modulatedDrive = std::max (0.01f, modulatedDrive);
                
                // 3. Dynamic Auto-Gain Calculation
                float autoGainLinear = 1.0f / static_cast<float>(std::sqrt(static_cast<double>(driveLinear)));

                // 4. Apply Drive
                x *= modulatedDrive;
                
                // 5. Transfer Functions
                if (type == DistortionType::Hard)
                    x = hardClip (x);
                else if (type == DistortionType::Foldback)
                    x = foldback (x);
                else if (type == DistortionType::Tube)
                    x = tubeClip (x);
                else if (type == DistortionType::Tape)
                    x = tapeClip (x);
                else if (type == DistortionType::Transistor)
                    x = transistorClip (x);
                else if (type == DistortionType::Custom)
                    x = customClip (x);
                else
                    x = softClip (x);

                // 6. Apply Compensated Gain
                channelData[sample] = x * autoGainLinear;
            }
            
            // Save state back to memory heap
            envelope[channel] = currentEnv;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
        }
    }

private:
    DistortionType type { DistortionType::Soft };
<<<<<<< HEAD
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
=======
    double sampleRate { 44100.0 };
    
    float driveLinear { 1.0f };
    float dynamicDepth { 0.0f };
    float attackCoef { 0.0f };
    float releaseCoef { 0.0f };
    
    std::vector<float> envelope;

    // --- Nonlinear Algorithms --- //

    inline float hardClip (float x) { return juce::jlimit (-1.0f, 1.0f, x); }
    inline float softClip (float x) { return x / (1.0f + std::abs (x)); }
    inline float foldback (float x) { return std::sin (x * juce::MathConstants<float>::halfPi); }
    inline float tubeClip (float x)
    {
        if (x > 0.0f) return x / (1.0f + x); 
        else return x / (1.0f + 0.25f * std::abs (x)); 
    }
    inline float tapeClip (float x)
    {
        if (x > -1.0f && x < 1.0f) return 1.5f * (x - ((x * x * x) / 3.0f));
        else return x >= 1.0f ? 1.0f : -1.0f;
    }
    inline float transistorClip (float x)
    {
        if (x > 0.0f) return x / (1.0f + x);
        else return (2.0f * x) / (1.0f - 2.0f * x);
    }
    inline float customClip (float x)
    {
        float clamped = juce::jlimit (-1.0f, 1.0f, x);
        return 4.0f * (clamped * clamped * clamped) - 3.0f * clamped;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    }
};

} // namespace DSP
} // namespace Othrys