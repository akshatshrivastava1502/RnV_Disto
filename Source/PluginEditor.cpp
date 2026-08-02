#include "PluginProcessor.h"
#include "PluginEditor.h"

// ==============================================================================
// Distortion Tab Component
// ==============================================================================
class DistortionTab : public juce::Component
{
public:
    DistortionTab (juce::AudioProcessorValueTreeState& apvts)
    {
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
        bounds.removeFromTop (20);
        
        const int knobWidth = 100;
        const int spacing = 15;

        auto knobRow = bounds.removeFromTop (130);
        
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
    juce::Slider inputGainSlider, driveSlider, toneSlider, outputGainSlider, mixSlider;
    juce::ComboBox typeSelector;
    juce::Label inputLabel, driveLabel, toneLabel, outputLabel, mixLabel, typeLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

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
        bounds.removeFromTop (20);
        
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
    juce::Slider mixSlider, sizeSlider, dampSlider, widthSlider;
    juce::ComboBox typeSelector;
    juce::Label mixLabel, sizeLabel, dampLabel, widthLabel, typeLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;

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
        bounds.removeFromTop (20);
        
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
    juce::Slider mixSlider, timeSlider, feedbackSlider;
    juce::Label mixLabel, timeLabel, feedbackLabel;

    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    std::unique_ptr<SliderAttachment> mixAttachment, timeAttachment, feedbackAttachment;
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
    addAndMakeVisible (tabs);

    setSize (700, 400);
}

RnVDistoAudioProcessorEditor::~RnVDistoAudioProcessorEditor()
{
    setLookAndFeel (nullptr);
}

void RnVDistoAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour::fromRGB (18, 18, 20));
}

void RnVDistoAudioProcessorEditor::resized()
{
    tabs.setBounds (getLocalBounds());
}