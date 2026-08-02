#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/CustomLookAndFeel.h"

class RnVDistoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    RnVDistoAudioProcessorEditor (RnVDistoAudioProcessor&);
    ~RnVDistoAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    RnVDistoAudioProcessor& audioProcessor;
    RnVDisto::GUI::CustomLookAndFeel customLookAndFeel;

    // --- Tabbed Component for Screens ---
    juce::TabbedComponent tabs;

    // --- Preset Buttons & Dropdown ---
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    juce::ComboBox presetSelector;

    void updatePresetComboBox();

    // --- File Chooser ---
    std::unique_ptr<juce::FileChooser> fileChooser;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RnVDistoAudioProcessorEditor)
};