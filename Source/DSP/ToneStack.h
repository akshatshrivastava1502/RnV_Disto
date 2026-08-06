#pragma once
#include <juce_dsp/juce_dsp.h>

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

} // namespace DSP
} // namespace Othrys