#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "BinaryData.h"

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
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, RnVDisto::Parameters::distortionPowerID, powerButton);

        auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text) 
        {
            slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
            addAndMakeVisible (slider);

            label.setText (text, juce::dontSendNotification);
            label.setJustificationType (juce::Justification::centred);
            addAndMakeVisible (label);
        };

        setupSlider (inputGainSlider, inputLabel, "Input Gain");
        setupSlider (driveSlider, driveLabel, "Drive");
        setupSlider (toneSlider, toneLabel, "Tone");
        setupSlider (outputGainSlider, outputLabel, "Output Gain");
        setupSlider (mixSlider, mixLabel, "Mix");

        addAndMakeVisible (typeSelector);
        typeSelector.addItemList ({"Soft Clip", "Hard Clip", "Foldback", "Tube", "Tape"}, 1);
        
        typeLabel.setText ("Algorithm", juce::dontSendNotification);
        typeLabel.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (typeLabel);

        inputGainAttachment  = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::inputGainID, inputGainSlider);
        driveAttachment      = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::driveID, driveSlider);
        toneAttachment       = std::make_unique<SliderAttachment>   (apvts, "tone", toneSlider);
        outputGainAttachment = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::outputGainID, outputGainSlider);
        mixAttachment        = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::mixID, mixSlider);
        typeAttachment       = std::make_unique<ComboBoxAttachment> (apvts, RnVDisto::Parameters::typeID, typeSelector);
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
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, RnVDisto::Parameters::reverbPowerID, powerButton);

        auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text) 
        {
            slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
            addAndMakeVisible (slider);

            label.setText (text, juce::dontSendNotification);
            label.setJustificationType (juce::Justification::centred);
            addAndMakeVisible (label);
        };

        setupSlider (mixSlider, mixLabel, "Mix");
        setupSlider (sizeSlider, sizeLabel, "Size");
        setupSlider (dampSlider, dampLabel, "Damping");
        setupSlider (widthSlider, widthLabel, "Width");

        addAndMakeVisible (typeSelector);
        typeSelector.addItemList ({"Room", "Hall"}, 1);
        
        typeLabel.setText ("Reverb Type", juce::dontSendNotification);
        typeLabel.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (typeLabel);

        mixAttachment   = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::reverbMixID, mixSlider);
        sizeAttachment  = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::reverbSizeID, sizeSlider);
        dampAttachment  = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::reverbDampID, dampSlider);
        widthAttachment = std::make_unique<SliderAttachment>   (apvts, RnVDisto::Parameters::reverbWidthID, widthSlider);
        typeAttachment  = std::make_unique<ComboBoxAttachment> (apvts, RnVDisto::Parameters::reverbTypeID, typeSelector);
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
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, RnVDisto::Parameters::delayPowerID, powerButton);

        auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text) 
        {
            slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
            addAndMakeVisible (slider);

            label.setText (text, juce::dontSendNotification);
            label.setJustificationType (juce::Justification::centred);
            addAndMakeVisible (label);
        };

        setupSlider (mixSlider, mixLabel, "Mix");
        setupSlider (timeSlider, timeLabel, "Time (ms)");
        setupSlider (feedbackSlider, feedbackLabel, "Feedback");

        mixAttachment      = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::delayMixID, mixSlider);
        timeAttachment     = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::delayTimeID, timeSlider);
        feedbackAttachment = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::delayFeedbackID, feedbackSlider);
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
        powerAttachment = std::make_unique<ButtonAttachment> (apvts, RnVDisto::Parameters::gatePowerID, powerButton);

        auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text) 
        {
            slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
            slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
            addAndMakeVisible (slider);

            label.setText (text, juce::dontSendNotification);
            label.setJustificationType (juce::Justification::centred);
            addAndMakeVisible (label);
        };

        setupSlider (thresholdSlider, thresholdLabel, "Threshold (dB)");
        setupSlider (attackSlider, attackLabel, "Attack (ms)");
        setupSlider (releaseSlider, releaseLabel, "Release (ms)");

        thresholdAttachment = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::gateThresholdID, thresholdSlider);
        attackAttachment    = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::gateAttackID, attackSlider);
        releaseAttachment   = std::make_unique<SliderAttachment> (apvts, RnVDisto::Parameters::gateReleaseID, releaseSlider);
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
// RnVDistoAudioProcessorEditor Implementation
// ==============================================================================
RnVDistoAudioProcessorEditor::RnVDistoAudioProcessorEditor (RnVDistoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), tabs (juce::TabbedButtonBar::TabsAtTop)
{
    setLookAndFeel (&customLookAndFeel);

    // Setup tabs
    tabs.addTab ("Distortion", juce::Colours::transparentBlack, new DistortionTab (audioProcessor.apvts), true);
    tabs.addTab ("Reverb", juce::Colours::transparentBlack, new ReverbTab (audioProcessor.apvts), true);
    tabs.addTab ("Delay", juce::Colours::transparentBlack, new DelayTab (audioProcessor.apvts), true);
    tabs.addTab ("Gate", juce::Colours::transparentBlack, new GateTab (audioProcessor.apvts), true);
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
    bypassAttachment = std::make_unique<ButtonAttachment> (audioProcessor.apvts, RnVDisto::Parameters::bypassID, bypassButton);

    // Setup input channel selector
    addAndMakeVisible (inputChannelSelector);
    inputChannelSelector.addItemList ({"Input: Both", "Input: 1", "Input: 2"}, 1);
    inputChannelAttachment = std::make_unique<ComboBoxAttachment> (audioProcessor.apvts, RnVDisto::Parameters::inputChannelModeID, inputChannelSelector);

    // Make the window wider to accommodate our tabs nicely
    setSize (800, 420);
}

RnVDistoAudioProcessorEditor::~RnVDistoAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void RnVDistoAudioProcessorEditor::paint (juce::Graphics& g)
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
    g.drawText ("RnV Disto", 280, 10, 120, 30, juce::Justification::centred, true);
}

void RnVDistoAudioProcessorEditor::resized()
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

void RnVDistoAudioProcessorEditor::updatePresetComboBox()
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