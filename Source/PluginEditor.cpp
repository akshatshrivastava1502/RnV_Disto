#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

namespace
{
// Helper function to configure a rotary slider and its label, reducing code duplication.
void setupRotarySlider (juce::Component& owner,
                        juce::Slider& slider,
                        juce::Label& label,
                        const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
    owner.addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    owner.addAndMakeVisible (label);
}

// Helper function to configure a vertical slider and its label, reducing code duplication.
void setupVerticalSlider (juce::Component& owner,
                          juce::Slider& slider,
                          juce::Label& label,
                          const juce::String& labelText)
{
    slider.setSliderStyle (juce::Slider::LinearVertical);
    slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 50, 20);
    owner.addAndMakeVisible (slider);

    label.setText (labelText, juce::dontSendNotification);
    label.setJustificationType (juce::Justification::centred);
    owner.addAndMakeVisible (label);
}
} // namespace
// ==============================================================================
// Distortion Tab Component
// ==============================================================================
class DistortionTab : public juce::Component
{
public:
    DistortionTab (juce::AudioProcessorValueTreeState& apvts)
    {
        powerButton.setButtonText ("Power");
        addAndMakeVisible (powerButton);
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, Othrys::Parameters::distortionPowerID, powerButton);

        setupRotarySlider (*this, inputGainSlider, inputLabel, "Input Gain");
        setupRotarySlider (*this, driveSlider, driveLabel, "Drive");
        setupRotarySlider (*this, toneSlider, toneLabel, "Tone");
        setupRotarySlider (*this, outputGainSlider, outputLabel, "Output Gain");
        setupRotarySlider (*this, mixSlider, mixLabel, "Mix");

        addAndMakeVisible (typeSelector);
        typeSelector.addItemList ({"Soft Clip", "Hard Clip", "Foldback", "Tube", "Tape"}, 1);
        
        typeLabel.setText ("Algorithm", juce::dontSendNotification);
        typeLabel.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (typeLabel);

        inputGainAttachment  = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::inputGainID, inputGainSlider);
        driveAttachment      = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::driveID, driveSlider);
        toneAttachment       = std::make_unique<SliderAttachment>   (apvts, "tone", toneSlider);
        outputGainAttachment = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::outputGainID, outputGainSlider);
        mixAttachment        = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::mixID, mixSlider);
        typeAttachment       = std::make_unique<ComboBoxAttachment> (apvts, Othrys::Parameters::typeID, typeSelector);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        powerButton.setBounds (bounds.removeFromTop (30).withSizeKeepingCentre (80, 25));
        bounds.removeFromTop (10); // spacing
        
        const int knobWidth = 100;
        const int spacing = 15;

        auto knobRow = bounds.removeFromTop (130).withSizeKeepingCentre (560, 130);
        
        inputGainSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        inputLabel.setBounds (inputGainSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        driveSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        driveLabel.setBounds (driveSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        toneSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        toneLabel.setBounds (toneSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        mixSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        mixLabel.setBounds (mixSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        outputGainSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        outputLabel.setBounds (outputGainSlider.getBounds().translated (0, -20).withHeight (20));

        auto bottomRow = bounds.removeFromTop (80).withSizeKeepingCentre (200, 30);
        typeLabel.setBounds (bottomRow.translated (0, -25).withHeight (20));
        typeSelector.setBounds (bottomRow);
    }

private:
    juce::ToggleButton powerButton;
    juce::Slider inputGainSlider, driveSlider, toneSlider, outputGainSlider, mixSlider;
    juce::ComboBox typeSelector;
    juce::Label inputLabel, driveLabel, toneLabel, outputLabel, mixLabel, typeLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<SliderAttachment> inputGainAttachment, driveAttachment, toneAttachment, outputGainAttachment, mixAttachment;
    std::unique_ptr<ComboBoxAttachment> typeAttachment;
};

// ==============================================================================
// Reverb Tab Component
// ==============================================================================
class ReverbTab : public juce::Component
{
public:
    ReverbTab (juce::AudioProcessorValueTreeState& apvts)
    {
        powerButton.setButtonText ("Power");
        addAndMakeVisible (powerButton);
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, Othrys::Parameters::reverbPowerID, powerButton);

        setupRotarySlider (*this, mixSlider, mixLabel, "Mix");
        setupRotarySlider (*this, sizeSlider, sizeLabel, "Size");
        setupRotarySlider (*this, dampSlider, dampLabel, "Damping");
        setupRotarySlider (*this, widthSlider, widthLabel, "Width");

        addAndMakeVisible (typeSelector);
        typeSelector.addItemList ({"Room", "Hall"}, 1);
        
        typeLabel.setText ("Reverb Type", juce::dontSendNotification);
        typeLabel.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (typeLabel);

        mixAttachment   = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::reverbMixID, mixSlider);
        sizeAttachment  = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::reverbSizeID, sizeSlider);
        dampAttachment  = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::reverbDampID, dampSlider);
        widthAttachment = std::make_unique<SliderAttachment>   (apvts, Othrys::Parameters::reverbWidthID, widthSlider);
        typeAttachment  = std::make_unique<ComboBoxAttachment> (apvts, Othrys::Parameters::reverbTypeID, typeSelector);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        powerButton.setBounds (bounds.removeFromTop (30).withSizeKeepingCentre (80, 25));
        bounds.removeFromTop (10); // spacing
        
        const int knobWidth = 100;
        const int spacing = 20;

        auto knobRow = bounds.removeFromTop (130).withSizeKeepingCentre (460, 130);
        
        mixSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        mixLabel.setBounds (mixSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        sizeSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        sizeLabel.setBounds (sizeSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        dampSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        dampLabel.setBounds (dampSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        widthSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        widthLabel.setBounds (widthSlider.getBounds().translated (0, -20).withHeight (20));

        auto bottomRow = bounds.removeFromTop (80).withSizeKeepingCentre (200, 30);
        typeLabel.setBounds (bottomRow.translated (0, -25).withHeight (20));
        typeSelector.setBounds (bottomRow);
    }

private:
    juce::ToggleButton powerButton;
    juce::Slider mixSlider, sizeSlider, dampSlider, widthSlider;
    juce::ComboBox typeSelector;
    juce::Label mixLabel, sizeLabel, dampLabel, widthLabel, typeLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment, sizeAttachment, dampAttachment, widthAttachment;
    std::unique_ptr<ComboBoxAttachment> typeAttachment;
};

// ==============================================================================
// Delay Tab Component
// ==============================================================================
class DelayTab : public juce::Component
{
public:
    DelayTab (juce::AudioProcessorValueTreeState& apvts)
    {
        powerButton.setButtonText ("Power");
        addAndMakeVisible (powerButton);
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, Othrys::Parameters::delayPowerID, powerButton);

        setupRotarySlider (*this, mixSlider, mixLabel, "Mix");
        setupRotarySlider (*this, timeSlider, timeLabel, "Time (ms)");
        setupRotarySlider (*this, feedbackSlider, feedbackLabel, "Feedback");

        mixAttachment      = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::delayMixID, mixSlider);
        timeAttachment     = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::delayTimeID, timeSlider);
        feedbackAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::delayFeedbackID, feedbackSlider);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        powerButton.setBounds (bounds.removeFromTop (30).withSizeKeepingCentre (80, 25));
        bounds.removeFromTop (10); // spacing
        
        const int knobWidth = 100;
        const int spacing = 20;

        auto knobRow = bounds.removeFromTop (130).withSizeKeepingCentre (340, 130);
        
        mixSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        mixLabel.setBounds (mixSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        timeSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        timeLabel.setBounds (timeSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        feedbackSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        feedbackLabel.setBounds (feedbackSlider.getBounds().translated (0, -20).withHeight (20));
    }

private:
    juce::ToggleButton powerButton;
    juce::Slider mixSlider, timeSlider, feedbackSlider;
    juce::Label mixLabel, timeLabel, feedbackLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<SliderAttachment> mixAttachment, timeAttachment, feedbackAttachment;
};

// ==============================================================================
// Gate Tab Component
// ==============================================================================
class GateTab : public juce::Component
{
public:
    GateTab (juce::AudioProcessorValueTreeState& apvts)
    {
        powerButton.setButtonText ("Power");
        addAndMakeVisible (powerButton);
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, Othrys::Parameters::gatePowerID, powerButton);

        setupRotarySlider (*this, thresholdSlider, thresholdLabel, "Threshold (dB)");
        setupRotarySlider (*this, attackSlider, attackLabel, "Attack (ms)");
        setupRotarySlider (*this, releaseSlider, releaseLabel, "Release (ms)");

        thresholdAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::gateThresholdID, thresholdSlider);
        attackAttachment    = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::gateAttackID, attackSlider);
        releaseAttachment   = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::gateReleaseID, releaseSlider);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        powerButton.setBounds (bounds.removeFromTop (30).withSizeKeepingCentre (80, 25));
        bounds.removeFromTop (10); // spacing
        
        const int knobWidth = 100;
        const int spacing = 20;

        auto knobRow = bounds.removeFromTop (130).withSizeKeepingCentre (340, 130);
        
        thresholdSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        thresholdLabel.setBounds (thresholdSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        attackSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        attackLabel.setBounds (attackSlider.getBounds().translated (0, -20).withHeight (20));
        knobRow.removeFromLeft (spacing);

        releaseSlider.setBounds (knobRow.removeFromLeft (knobWidth));
        releaseLabel.setBounds (releaseSlider.getBounds().translated (0, -20).withHeight (20));
    }

private:
    juce::ToggleButton powerButton;
    juce::Slider thresholdSlider, attackSlider, releaseSlider;
    juce::Label thresholdLabel, attackLabel, releaseLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<SliderAttachment> thresholdAttachment, attackAttachment, releaseAttachment;
};

// ==============================================================================
// EQ Tab Component
// ==============================================================================
class EQTab : public juce::Component
{
public:
    EQTab (juce::AudioProcessorValueTreeState& apvts)
    {
        powerButton.setButtonText ("Power");
        addAndMakeVisible (powerButton);
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, Othrys::Parameters::eqPowerID, powerButton);

        setupVerticalSlider (*this, hpfFreqSlider, hpfFreqLabel, "HPF");
        setupVerticalSlider (*this, band65HzSlider, band65HzLabel, "65Hz");
        setupVerticalSlider (*this, band125HzSlider, band125HzLabel, "125Hz");
        setupVerticalSlider (*this, band250HzSlider, band250HzLabel, "250Hz");
        setupVerticalSlider (*this, band500HzSlider, band500HzLabel, "500Hz");
        setupVerticalSlider (*this, band1kHzSlider, band1kHzLabel, "1kHz");
        setupVerticalSlider (*this, band2kHzSlider, band2kHzLabel, "2kHz");
        setupVerticalSlider (*this, band4kHzSlider, band4kHzLabel, "4kHz");
        setupVerticalSlider (*this, band8kHzSlider, band8kHzLabel, "8kHz");
        setupVerticalSlider (*this, band16kHzSlider, band16kHzLabel, "16kHz");
        setupVerticalSlider (*this, lpfFreqSlider, lpfFreqLabel, "LPF");

        hpfFreqAttachment   = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqHpfFreqID, hpfFreqSlider);
        lpfFreqAttachment   = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqLpfFreqID, lpfFreqSlider);
        band65HzAttachment  = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand65HzID, band65HzSlider);
        band125HzAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand125HzID, band125HzSlider);
        band250HzAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand250HzID, band250HzSlider);
        band500HzAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand500HzID, band500HzSlider);
        band1kHzAttachment  = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand1kHzID, band1kHzSlider);
        band2kHzAttachment  = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand2kHzID, band2kHzSlider);
        band4kHzAttachment  = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand4kHzID, band4kHzSlider);
        band8kHzAttachment  = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand8kHzID, band8kHzSlider);
        band16kHzAttachment = std::make_unique<SliderAttachment> (apvts, Othrys::Parameters::eqBand16kHzID, band16kHzSlider);
    }

    void resized() override
    {
        auto bounds = getLocalBounds().reduced (20);
        powerButton.setBounds (bounds.removeFromTop (30).withSizeKeepingCentre (80, 25));
        bounds.removeFromTop (10); // spacing
        
        const int sliderWidth = 45; // Slightly reduced width for better fit
        const int sliderHeight = 150; // Height of the slider itself
        const int labelHeight = 20; // Height for the label below the slider
        const int spacing = 8; // Spacing between sliders

        // Calculate total width needed for all 11 sliders and 10 spacings
        const int totalSliders = 11;
        const int totalSpacing = totalSliders - 1;
        const int totalContentWidth = (totalSliders * sliderWidth) + (totalSpacing * spacing);

        // Create a centered rectangle for the entire row of sliders + labels
        auto contentArea = bounds.removeFromTop (sliderHeight + labelHeight);
        auto centeredSliderRow = contentArea.withSizeKeepingCentre (totalContentWidth, sliderHeight);

        // HPF
        hpfFreqSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        hpfFreqLabel.setBounds (hpfFreqSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);
        
        // 9 Bands
        band65HzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band65HzLabel.setBounds (band65HzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band125HzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band125HzLabel.setBounds (band125HzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band250HzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band250HzLabel.setBounds (band250HzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band500HzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band500HzLabel.setBounds (band500HzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band1kHzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band1kHzLabel.setBounds (band1kHzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band2kHzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band2kHzLabel.setBounds (band2kHzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band4kHzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band4kHzLabel.setBounds (band4kHzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band8kHzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band8kHzLabel.setBounds (band8kHzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        band16kHzSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        band16kHzLabel.setBounds (band16kHzSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
        centeredSliderRow.removeFromLeft (spacing);

        // LPF
        lpfFreqSlider.setBounds (centeredSliderRow.removeFromLeft (sliderWidth));
        lpfFreqLabel.setBounds (lpfFreqSlider.getBounds().translated (0, -labelHeight).withHeight (labelHeight));
    }

private:
    juce::ToggleButton powerButton;
    juce::Slider hpfFreqSlider, lpfFreqSlider;
    juce::Slider band65HzSlider, band125HzSlider, band250HzSlider, band500HzSlider, band1kHzSlider, band2kHzSlider, band4kHzSlider, band8kHzSlider, band16kHzSlider;
    juce::Label hpfFreqLabel, lpfFreqLabel;
    juce::Label band65HzLabel, band125HzLabel, band250HzLabel, band500HzLabel, band1kHzLabel, band2kHzLabel, band4kHzLabel, band8kHzLabel, band16kHzLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ButtonAttachment = juce::AudioProcessorValueTreeState::ButtonAttachment;

    std::unique_ptr<ButtonAttachment> powerAttachment;
    std::unique_ptr<SliderAttachment> hpfFreqAttachment, lpfFreqAttachment, band65HzAttachment, band125HzAttachment, band250HzAttachment, band500HzAttachment, band1kHzAttachment, band2kHzAttachment, band4kHzAttachment, band8kHzAttachment, band16kHzAttachment;
};

// ==============================================================================
// OthrysAudioProcessorEditor Implementation
// ==============================================================================
OthrysAudioProcessorEditor::OthrysAudioProcessorEditor (OthrysAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), tabs (juce::TabbedButtonBar::TabsAtTop)
{
    setLookAndFeel (&customLookAndFeel);

    // Setup tabs
    tabs.addTab ("Distortion", juce::Colours::transparentBlack, new DistortionTab (audioProcessor.apvts), true);
    tabs.addTab ("Reverb", juce::Colours::transparentBlack, new ReverbTab (audioProcessor.apvts), true);
    tabs.addTab ("Delay", juce::Colours::transparentBlack, new DelayTab (audioProcessor.apvts), true);
    tabs.addTab ("Gate", juce::Colours::transparentBlack, new GateTab (audioProcessor.apvts), true);
    tabs.addTab ("EQ", juce::Colours::transparentBlack, new EQTab (audioProcessor.apvts), true);
    addAndMakeVisible (tabs);

    // Setup Preset Selector Dropdown
    addAndMakeVisible (presetSelector);
    updatePresetComboBox();
    presetSelector.onChange = [this]() {
        auto index = presetSelector.getSelectedItemIndex();
        if (index >= 0)
        {
            auto files = audioProcessor.getPresetFiles();
            if (index < files.size())
            {
                audioProcessor.loadPresetFromFile (files[index]);
            }
        }
    };

    // Setup Preset Buttons
    savePresetButton.setButtonText ("Save Preset");
    addAndMakeVisible (savePresetButton);
    savePresetButton.onClick = [this]() {
        fileChooser = std::make_unique<juce::FileChooser> (
            "Save Preset",
            audioProcessor.getPresetsFolder(),
            "*.rnv"
        );
        fileChooser->launchAsync (juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles,
            [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file != juce::File{})
                {
                    if (file.getFileExtension() != ".rnv")
                        file = file.withFileExtension (".rnv");
                    
                    audioProcessor.savePresetToFile (file);
                    updatePresetComboBox();
                }
            });
    };

    loadPresetButton.setButtonText ("Load Preset");
    addAndMakeVisible (loadPresetButton);
    loadPresetButton.onClick = [this]() {
        fileChooser = std::make_unique<juce::FileChooser> (
            "Load Preset",
            audioProcessor.getPresetsFolder(),
            "*.rnv;*.xml"
        );
        fileChooser->launchAsync (juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this] (const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    audioProcessor.loadPresetFromFile (file);
                    updatePresetComboBox();
                }
            });
    };

    bypassButton.setButtonText ("Bypass");
    addAndMakeVisible (bypassButton);
    bypassAttachment = std::make_unique<ButtonAttachment> (audioProcessor.apvts, Othrys::Parameters::bypassID, bypassButton);

    // Setup input channel selector
    addAndMakeVisible (inputChannelSelector);
    inputChannelSelector.addItemList ({"Input: Both", "Input: 1", "Input: 2"}, 1);
    inputChannelAttachment = std::make_unique<ComboBoxAttachment> (audioProcessor.apvts, Othrys::Parameters::inputChannelModeID, inputChannelSelector);

    // Make the window wider to accommodate our tabs nicely
    setSize (800, 420);
}

OthrysAudioProcessorEditor::~OthrysAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void OthrysAudioProcessorEditor::paint (juce::Graphics& g)
{
    auto bgImg = juce::ImageCache::getFromMemory (BinaryData::panel_background_jpg, BinaryData::panel_background_jpgSize);
    
    if (bgImg.isValid())
    {
        g.drawImageWithin (bgImg, 0, 0, getWidth(), getHeight(), juce::RectanglePlacement::stretchToFit, false);
    }
    else
    {
        g.fillAll (juce::Colour::fromRGB (18, 18, 20));
    }

    // Draw the Header Title (positioned centrally in top bar)
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (18.0f));
    g.drawText ("Othrys", 280, 10, 120, 30, juce::Justification::centred, true);
}

void OthrysAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Top Bar Layout
    auto topBar = bounds.removeFromTop (50);
    
    // Position Bypass button on the left of top bar
    bypassButton.setBounds (topBar.removeFromLeft (150).reduced (5));
    
    // Position Input Channel Selector next to it
    inputChannelSelector.setBounds (topBar.removeFromLeft (130).reduced (5));
    
    // Position Save/Load buttons and ComboBox on the right of top bar
    loadPresetButton.setBounds (topBar.removeFromRight (100).reduced (5));
    savePresetButton.setBounds (topBar.removeFromRight (100).reduced (5));
    presetSelector.setBounds (topBar.removeFromRight (200).reduced (5));

    // Tabbed component takes the rest
    tabs.setBounds (bounds);
}

void OthrysAudioProcessorEditor::updatePresetComboBox()
{
    presetSelector.clear (juce::dontSendNotification);
    
    auto files = audioProcessor.getPresetFiles();
    int id = 1;
    for (auto& file : files)
    {
        presetSelector.addItem (file.getFileNameWithoutExtension(), id++);
    }
    
    presetSelector.setText ("Select Preset...", juce::dontSendNotification);
}