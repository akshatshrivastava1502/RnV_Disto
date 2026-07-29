#pragma once

#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"

class RnVDistoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RnVDistoAudioProcessorEditor (RnVDistoAudioProcessor&);
    ~RnVDistoAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RnVDistoAudioProcessor& audioProcessor;

    // --- UI Components ---
    juce::Slider inputGainSlider, driveSlider, toneSlider, outputGainSlider, mixSlider;
    juce::ComboBox typeSelector;
    
    // --- Labels ---
    juce::Label inputLabel, driveLabel, toneLabel, outputLabel, mixLabel, typeLabel;

    // --- APVTS Attachments (The bridge between GUI and DSP) ---
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<SliderAttachment> inputGainAttachment;
    std::unique_ptr<SliderAttachment> driveAttachment;
    std::unique_ptr<SliderAttachment> toneAttachment;
    std::unique_ptr<SliderAttachment> outputGainAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment;
    std::unique_ptr<ComboBoxAttachment> typeAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RnVDistoAudioProcessorEditor)
};