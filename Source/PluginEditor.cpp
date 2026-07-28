#include "PluginProcessor.h"
#include "PluginEditor.h"

RnVDistoAudioProcessorEditor::RnVDistoAudioProcessorEditor (RnVDistoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Define the initial UI bounds
    setSize (600, 400);
}

RnVDistoAudioProcessorEditor::~RnVDistoAudioProcessorEditor()
{
}

void RnVDistoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // Dark theme background
    g.fillAll (juce::Colour::fromRGB (20, 20, 22));

    g.setColour (juce::Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("RnV Disto - Foundation Set", getLocalBounds(), juce::Justification::centered, 1);
}

void RnVDistoAudioProcessorEditor::resized()
{
    // UI component layout logic will go here
}