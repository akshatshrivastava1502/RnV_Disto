#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
#include "PluginProcessor.h"
#include "GUI/CustomLookAndFeel.h"

class OthrysAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    OthrysAudioProcessorEditor (OthrysAudioProcessor&);
    ~OthrysAudioProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    OthrysAudioProcessor& audioProcessor;
    Othrys::GUI::CustomLookAndFeel customLookAndFeel;

    // --- Tabbed Component for Screens ---
    juce::TabbedComponent tabs;

    // --- Preset Buttons & Dropdown ---
    juce::TextButton savePresetButton;
    juce::TextButton loadPresetButton;
    juce::ComboBox presetSelector;

    void updatePresetComboBox();

    // --- File Chooser ---
    std::unique_ptr<juce::FileChooser> fileChooser;

    // --- Top Bar Controls ---
    juce::ToggleButton bypassButton;
    juce::ComboBox inputChannelSelector;

    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

    std::unique_ptr<ButtonAttachment> bypassAttachment;
    std::unique_ptr<ComboBoxAttachment> inputChannelAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OthrysAudioProcessorEditor)
};