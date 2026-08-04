#pragma once
#include <juce_dsp/juce_dsp.h>

namespace Othrys
{
namespace DSP
{
    class Reverb
    {
    public:
        Reverb() = default;
        ~Reverb() = default;

        void prepare (const juce::dsp::ProcessSpec& spec)
        {
            reverb.prepare (spec);
            reset();
        }

        void updateParameters (int type, float mixPct, float size, float damp, float width)
        {
            // type: 0 = Room, 1 = Hall
            juce::dsp::Reverb::Parameters params;
            
            if (type == 0) // Room
            {
                // Scaled down room size and width for a tighter room sound
                params.roomSize = size * 0.4f + 0.1f;
                params.damping = damp * 0.8f;
                params.width = width * 0.5f + 0.1f;
            }
            else // Hall
            {
                // Larger room size and wider image for a hall sound
                params.roomSize = size * 0.7f + 0.25f;
                params.damping = damp * 0.4f;
                params.width = width * 0.7f + 0.3f;
            }

            // Map wet/dry levels based on mix percentage
            float mixVal = mixPct / 100.0f;
            params.wetLevel = mixVal * 0.4f; // Scale slightly down to avoid distortion
            params.dryLevel = 1.0f - mixVal;

            reverb.setParameters (params);
        }

        void process (const juce::dsp::ProcessContextReplacing<float>& context)
        {
            reverb.process (context);
        }

        void reset()
        {
            reverb.reset();
        }

    private:
        juce::dsp::Reverb reverb;
    };
}
}
