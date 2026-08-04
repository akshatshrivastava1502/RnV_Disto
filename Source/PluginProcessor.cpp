#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
OthrysAudioProcessor::OthrysAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       apvts(*this, nullptr, "Parameters", createParameterLayout()) // Correctly initializing APVTS
#endif
{
    // Bind atomic pointers directly to APVTS state for lock-free audio thread access
    inputGainParam  = apvts.getRawParameterValue("input_gain");
    driveParam      = apvts.getRawParameterValue("drive");
    toneParam       = apvts.getRawParameterValue("tone");
    hpfCutoffParam  = apvts.getRawParameterValue("hpf");
    lpfCutoffParam  = apvts.getRawParameterValue("lpf");
    outputGainParam = apvts.getRawParameterValue("output_gain");
    dryWetParam     = apvts.getRawParameterValue("dry_wet");
}

OthrysAudioProcessor::~OthrysAudioProcessor()
{
}

//==============================================================================
const juce::String OthrysAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool OthrysAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool OthrysAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool OthrysAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double OthrysAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int OthrysAudioProcessor::getNumPrograms()
{
    return 1;
}

int OthrysAudioProcessor::getCurrentProgram()
{
    return 0;
}

void OthrysAudioProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused(index);
}

const juce::String OthrysAudioProcessor::getProgramName (int index)
{
    juce::ignoreUnused(index);
    return {};
}

void OthrysAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void OthrysAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = static_cast<juce::uint32>(getTotalNumOutputChannels());

    // Prepare Phase 2 & 4 DSP Modules
    dcBlocker.prepare(spec.sampleRate, spec.maximumBlockSize, spec.numChannels);
    noiseGate.prepare(spec);
    toneStack.prepare(spec);
    dryWetMixer.prepare(spec);
    
    // Initialize Oversampler
    oversampler.reset();
    oversampler.initProcessing(static_cast<size_t>(samplesPerBlock));
    
    // Prepare Distortion at the oversampled rate
    juce::dsp::ProcessSpec oversampledSpec = spec;
    oversampledSpec.sampleRate = sampleRate * oversampler.getOversamplingFactor();
    oversampledSpec.maximumBlockSize = static_cast<juce::uint32>(static_cast<size_t>(samplesPerBlock) * oversampler.getOversamplingFactor());
    distortion.prepare(oversampledSpec);
}

void OthrysAudioProcessor::releaseResources()
{
    oversampler.reset();
}

bool OthrysAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}

void OthrysAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Update DSP parameters from APVTS (Lock-free)
    float currentInputGain = juce::Decibels::decibelsToGain(inputGainParam->load());
    float currentOutputGain = juce::Decibels::decibelsToGain(outputGainParam->load());
    
    distortion.setDrive(driveParam->load());
    toneStack.setTone(toneParam->load());
    toneStack.setFilters(hpfCutoffParam->load(), lpfCutoffParam->load());
    
    dryWetMixer.setWetMixProportion(dryWetParam->load() / 100.0f);

    juce::dsp::AudioBlock<float> block(buffer);
    
    // 1. Input Gain
    buffer.applyGain(currentInputGain);
    
    // 2. DC Blocker & Noise Gate (Sanitization)
    dcBlocker.process(buffer);
    noiseGate.process(block);
    
    // 3. Dry/Wet Mix Reference (Captured after sanitization)
    dryWetMixer.pushDrySamples(block);
    
    // 4. Oversampling Up
    juce::dsp::AudioBlock<float> oversampledBlock = oversampler.processSamplesUp(block);
    
    // 5. Distortion (Runs on oversampled block)
    distortion.process(oversampledBlock);
    
    // 6. Oversampling Down
    oversampler.processSamplesDown(block);
    
    // 7. Tone & Filters
    toneStack.process(block);
    
    // 8. Output Gain
    buffer.applyGain(currentOutputGain);
    
    // 9. Dry/Wet Mix Output
    dryWetMixer.mixWetSamples(block);
}

//==============================================================================
bool OthrysAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OthrysAudioProcessor::createEditor()
{
    return new OthrysAudioProcessorEditor (*this);
}

//==============================================================================
// Clean, Standard JUCE State Management (Replaces the hallucinated lambda loop)
void OthrysAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void OthrysAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// Preset Management Implementations
juce::File OthrysAudioProcessor::getPresetsFolder()
{
    juce::File rootFolder = juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory);
    juce::File presetsFolder = rootFolder.getChildFile("Othrys").getChildFile("Presets");
    
    if (!presetsFolder.exists())
        presetsFolder.createDirectory();
        
    return presetsFolder;
}

void OthrysAudioProcessor::savePresetToFile(const juce::String& presetName)
{
    juce::File presetFile = getPresetsFolder().getChildFile(presetName + ".xml");
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    xml->writeTo(presetFile);
}

void OthrysAudioProcessor::loadPresetFromFile(const juce::File& presetFile)
{
    if (!presetFile.existsAsFile()) return;
    
    std::unique_ptr<juce::XmlElement> xmlState = juce::XmlDocument::parse(presetFile);
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

juce::Array<juce::File> OthrysAudioProcessor::getPresetFiles()
{
    return getPresetsFolder().findChildFiles(juce::File::TypesOfFileToFind::findFiles, false, "*.xml");
}

//==============================================================================
juce::AudioProcessorValueTreeState::ParameterLayout OthrysAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("input_gain", 1), "Input Gain", -24.0f, 24.0f, 0.0f));
        
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("drive", 1), "Drive", 0.0f, 100.0f, 0.0f));
        
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("tone", 1), "Tone", 0.0f, 1.0f, 0.5f));
        
    // Logarithmic skew for filters to ensure professional knob response
    juce::NormalisableRange<float> hpfRange(20.0f, 2000.0f, 1.0f);
    hpfRange.setSkewForCentre(200.0f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("hpf", 1), "HPF", hpfRange, 20.0f));

    juce::NormalisableRange<float> lpfRange(2000.0f, 20000.0f, 1.0f);
    lpfRange.setSkewForCentre(10000.0f);
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("lpf", 1), "LPF", lpfRange, 20000.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("output_gain", 1), "Output Gain", -24.0f, 24.0f, 0.0f));
        
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("dry_wet", 1), "Mix", 0.0f, 100.0f, 100.0f));

    return { params.begin(), params.end() };
}

//==============================================================================
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OthrysAudioProcessor();
}