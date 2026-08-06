#pragma once
#include <juce_dsp/juce_dsp.h>

namespace Othrys
{
namespace DSP
{

    class GraphicEQ
    {
    public:
        GraphicEQ()
        {
            // Initialize smoothed values with a default smoothing time
            const double defaultSmoothingTime = 0.05; // 50ms
            smoothedHpfFreq.reset (44100.0, defaultSmoothingTime);
            smoothedLpfFreq.reset (44100.0, defaultSmoothingTime);
            for (auto& smoothedGain : smoothedBandGainsDb)
                smoothedGain.reset (44100.0, defaultSmoothingTime);
        }

        ~GraphicEQ() = default;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            sampleRate = spec.sampleRate;
            eqChain.prepare (spec);

            // Update smoothed value sample rates
            smoothedHpfFreq.reset (sampleRate, 0.05);
            smoothedLpfFreq.reset (sampleRate, 0.05);
            for (auto& smoothedGain : smoothedBandGainsDb)
                smoothedGain.reset (sampleRate, 0.05);

            reset();
        }

        void updateParameters (float hpfFreq, float lpfFreq,
                               float gain65Hz, float gain125Hz, float gain250Hz,
                               float gain500Hz, float gain1kHz, float gain2kHz,
                               float gain4kHz, float gain8kHz, float gain16kHz)
        {
            smoothedHpfFreq.setTargetValue (hpfFreq);
            smoothedLpfFreq.setTargetValue (lpfFreq);
            smoothedBandGainsDb[0].setTargetValue (gain65Hz);
            smoothedBandGainsDb[1].setTargetValue (gain125Hz);
            smoothedBandGainsDb[2].setTargetValue (gain250Hz);
            smoothedBandGainsDb[3].setTargetValue (gain500Hz);
            smoothedBandGainsDb[4].setTargetValue (gain1kHz);
            smoothedBandGainsDb[5].setTargetValue (gain2kHz);
            smoothedBandGainsDb[6].setTargetValue (gain4kHz);
            smoothedBandGainsDb[7].setTargetValue (gain8kHz);
            smoothedBandGainsDb[8].setTargetValue (gain16kHz);
        }

        void process (const juce::dsp::ProcessContextReplacing<float>& context)
        {
            updateFilterCoefficients(); // Update coefficients smoothly at the start of each block
            eqChain.process (context);
        }

        void reset()
        {
            eqChain.reset();
            // Set initial smoothed values to their defaults
            smoothedHpfFreq.setCurrentAndTargetValue (20.0f);
            smoothedLpfFreq.setCurrentAndTargetValue (20000.0f);
            for (auto& smoothedGain : smoothedBandGainsDb)
                smoothedGain.setCurrentAndTargetValue (0.0f);
        }

    private:
        void updateFilterCoefficients()
        {
            if (sampleRate <= 0.0) return;

            // Get smoothed values
            const float hpfFreq = smoothedHpfFreq.getNextValue();
            const float lpfFreq = smoothedLpfFreq.getNextValue();
            const float gainsDb[9] = {
                smoothedBandGainsDb[0].getNextValue(),
                smoothedBandGainsDb[1].getNextValue(),
                smoothedBandGainsDb[2].getNextValue(),
                smoothedBandGainsDb[3].getNextValue(),
                smoothedBandGainsDb[4].getNextValue(),
                smoothedBandGainsDb[5].getNextValue(),
                smoothedBandGainsDb[6].getNextValue(),
                smoothedBandGainsDb[7].getNextValue(),
                smoothedBandGainsDb[8].getNextValue()
            };

            // Update HPF
            *eqChain.get<hpfIndex>().state = *juce::dsp::IIR::Coefficients<float>::makeHighPass (sampleRate, hpfFreq, 0.707f);

            // Update Peak Filters
            *eqChain.get<band65HzIndex>().state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 65.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[0]));
            *eqChain.get<band125HzIndex>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 125.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[1]));
            *eqChain.get<band250HzIndex>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 250.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[2]));
            *eqChain.get<band500HzIndex>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 500.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[3]));
            *eqChain.get<band1kHzIndex>().state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 1000.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[4]));
            *eqChain.get<band2kHzIndex>().state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 2000.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[5]));
            *eqChain.get<band4kHzIndex>().state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 4000.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[6]));
            *eqChain.get<band8kHzIndex>().state  = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 8000.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[7]));
            *eqChain.get<band16kHzIndex>().state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter (sampleRate, 16000.0f, 0.707f, juce::Decibels::decibelsToGain (gainsDb[8]));

            // Update LPF
            *eqChain.get<lpfIndex>().state = *juce::dsp::IIR::Coefficients<float>::makeLowPass (sampleRate, lpfFreq, 0.707f);
        }

        double sampleRate = 44100.0;

        // Using ProcessorDuplicator to handle stereo processing for each filter
        using FilterType = juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>>;

        // Define indices for the ProcessorChain
        enum
        {
            hpfIndex,
            band65HzIndex,
            band125HzIndex,
            band250HzIndex,
            band500HzIndex,
            band1kHzIndex,
            band2kHzIndex,
            band4kHzIndex,
            band8kHzIndex,
            band16kHzIndex,
            lpfIndex,
            numFilters // Keep track of total number of filters
        };

        // ProcessorChain to hold all filters
        juce::dsp::ProcessorChain<FilterType, FilterType, FilterType, FilterType, FilterType,
                                  FilterType, FilterType, FilterType, FilterType, FilterType,
                                  FilterType> eqChain;

        // Smoothed values for parameters to prevent zipper noise
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedHpfFreq;
        juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear> smoothedLpfFreq;
        std::array<juce::SmoothedValue<float, juce::ValueSmoothingTypes::Linear>, 9> smoothedBandGainsDb;
    };

} // namespace DSP
} // namespace Othrys