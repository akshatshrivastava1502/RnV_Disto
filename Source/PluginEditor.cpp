#include "PluginProcessor.h"
#include "PluginEditor.h"

RnVDistoAudioProcessorEditor::RnVDistoAudioProcessorEditor (RnVDistoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setLookAndFeel (&customLookAndFeel);
    // A quick lambda function to style our knobs cleanly without repetitive code
    auto setupSlider = [this](juce::Slider& slider, juce::Label& label, const juce::String& text) 
    {
        slider.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        slider.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 70, 20);
        addAndMakeVisible (slider);

        label.setText (text, juce::dontSendNotification);
        label.setJustificationType (juce::Justification::centred);
        addAndMakeVisible (label);
    };

    // Initialize all sliders
    setupSlider (inputGainSlider, inputLabel, "Input Gain");
    setupSlider (driveSlider, driveLabel, "Drive");
    setupSlider (toneSlider, toneLabel, "Tone");
    setupSlider (outputGainSlider, outputLabel, "Output Gain");
    setupSlider (mixSlider, mixLabel, "Mix");

    // Initialize ComboBox for Distortion Modes
    addAndMakeVisible (typeSelector);
    typeSelector.addItemList ({"Soft Clip", "Hard Clip", "Foldback", "Tube", "Tape"}, 1);
    
    typeLabel.setText ("Algorithm", juce::dontSendNotification);
    typeLabel.setJustificationType (juce::Justification::centred);
    addAndMakeVisible (typeLabel);

    // --- Wire up the Attachments to the APVTS ---
    inputGainAttachment  = std::make_unique<SliderAttachment>   (audioProcessor.apvts, RnVDisto::Parameters::inputGainID, inputGainSlider);
    driveAttachment      = std::make_unique<SliderAttachment>   (audioProcessor.apvts, RnVDisto::Parameters::driveID, driveSlider);
    toneAttachment       = std::make_unique<SliderAttachment>   (audioProcessor.apvts, "tone", toneSlider);
    outputGainAttachment = std::make_unique<SliderAttachment>   (audioProcessor.apvts, RnVDisto::Parameters::outputGainID, outputGainSlider);
    mixAttachment        = std::make_unique<SliderAttachment>   (audioProcessor.apvts, RnVDisto::Parameters::mixID, mixSlider);
    typeAttachment       = std::make_unique<ComboBoxAttachment> (audioProcessor.apvts, RnVDisto::Parameters::typeID, typeSelector);

    // Make the window wider to accommodate our row of knobs
    setSize (700, 400);
}

RnVDistoAudioProcessorEditor::~RnVDistoAudioProcessorEditor()
{
    // CRITICAL: We must detach the LookAndFeel before the window is destroyed
    setLookAndFeel (nullptr);
}

void RnVDistoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Deep dark premium background
    g.fillAll (juce::Colour::fromRGB (18, 18, 20));

    // Draw the Header
    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (15.0f));
    g.drawText ("RnV Disto", getLocalBounds().removeFromTop (70), juce::Justification::centred, true);
}

void RnVDistoAudioProcessorEditor::resized()
{
    auto bounds = getLocalBounds().reduced (30);
    bounds.removeFromTop (60); // Leave space for the header text

    const int knobWidth = 110;
    const int spacing = 20;

    // Layout the row of 5 primary knobs
    auto knobRow = bounds.removeFromTop (140);
    
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

    // Layout the Algorithm Selector in the bottom center
    auto bottomRow = bounds.removeFromTop (100).withSizeKeepingCentre (200, 30);
    typeLabel.setBounds (bottomRow.translated (0, -25).withHeight (20));
    typeSelector.setBounds (bottomRow);
}