#include "PluginProcessor.h"
#include "PluginEditor.h"

RnVDistoAudioProcessor::RnVDistoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     )
#endif
{
}

RnVDistoAudioProcessor::~RnVDistoAudioProcessor()
{
}

const juce::String RnVDistoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool RnVDistoAudioProcessor::acceptsMidi() const { return false; }
bool RnVDistoAudioProcessor::producesMidi() const { return false; }
bool RnVDistoAudioProcessor::isMidiEffect() const { return false; }
double RnVDistoAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int RnVDistoAudioProcessor::getNumPrograms() { return 1; }
int RnVDistoAudioProcessor::getCurrentProgram() { return 0; }
void RnVDistoAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }
const juce::String RnVDistoAudioProcessor::getProgramName (int index) { juce::ignoreUnused (index); return {}; }
void RnVDistoAudioProcessor::changeProgramName (int index, const juce::String& newName) { juce::ignoreUnused (index, newName); }

void RnVDistoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::ignoreUnused (sampleRate, samplesPerBlock);
    // DSP initialization will go here
}

void RnVDistoAudioProcessor::releaseResources()
{
    // Free up any resources here
}

bool RnVDistoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
    
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;

    return true
}

void RnVDistoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear output channels that don't contain input data to prevent feedback noise
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // DSP processing will go here
}

bool RnVDistoAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* RnVDistoAudioProcessor::createEditor()
{
    return new RnVDistoAudioProcessorEditor (*this);
}

void RnVDistoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::ignoreUnused (destData);
}

void RnVDistoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::ignoreUnused (data, sizeInBytes);
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RnVDistoAudioProcessor();
}