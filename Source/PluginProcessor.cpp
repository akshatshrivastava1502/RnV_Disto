#include "PluginProcessor.h"
<<<<<<< HEAD

// --- Add this explicit include ---
#include "PluginEditor.h"
// ---------------------------------

// ... rest of your implementation remains the same
OthrysAudioProcessor::OthrysAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     ),
       // Initialize the APVTS with our custom layout
       apvts (*this, nullptr, "Parameters", Othrys::Parameters::createParameterLayout())
#endif
{
    // Cache the atomic pointers here to avoid string lookups on the audio thread
    inputGainParam  = apvts.getRawParameterValue (Othrys::Parameters::inputGainID);
    driveParam      = apvts.getRawParameterValue (Othrys::Parameters::driveID);
    typeParam       = apvts.getRawParameterValue (Othrys::Parameters::typeID);
    toneParam       = apvts.getRawParameterValue ("tone");
    outputGainParam = apvts.getRawParameterValue (Othrys::Parameters::outputGainID);
    mixParam        = apvts.getRawParameterValue (Othrys::Parameters::mixID);

    // Reverb parameters
    reverbTypeParam  = apvts.getRawParameterValue (Othrys::Parameters::reverbTypeID);
    reverbMixParam   = apvts.getRawParameterValue (Othrys::Parameters::reverbMixID);
    reverbSizeParam  = apvts.getRawParameterValue (Othrys::Parameters::reverbSizeID);
    reverbDampParam  = apvts.getRawParameterValue (Othrys::Parameters::reverbDampID);
    reverbWidthParam = apvts.getRawParameterValue (Othrys::Parameters::reverbWidthID);

    // Delay parameters
    delayMixParam      = apvts.getRawParameterValue (Othrys::Parameters::delayMixID);
    delayTimeParam     = apvts.getRawParameterValue (Othrys::Parameters::delayTimeID);
    delayFeedbackParam = apvts.getRawParameterValue (Othrys::Parameters::delayFeedbackID);

    // Noise Gate parameters
    gateThresholdParam = apvts.getRawParameterValue (Othrys::Parameters::gateThresholdID);
    gateAttackParam    = apvts.getRawParameterValue (Othrys::Parameters::gateAttackID);
    gateReleaseParam   = apvts.getRawParameterValue (Othrys::Parameters::gateReleaseID);

    // Power parameters
    bypassParam          = apvts.getRawParameterValue (Othrys::Parameters::bypassID);
    distortionPowerParam = apvts.getRawParameterValue (Othrys::Parameters::distortionPowerID);
    reverbPowerParam     = apvts.getRawParameterValue (Othrys::Parameters::reverbPowerID);
    delayPowerParam      = apvts.getRawParameterValue (Othrys::Parameters::delayPowerID);
    gatePowerParam       = apvts.getRawParameterValue (Othrys::Parameters::gatePowerID);

    // EQ Parameters
    eqPowerParam   = apvts.getRawParameterValue (Othrys::Parameters::eqPowerID);
    eqHpfFreqParam = apvts.getRawParameterValue (Othrys::Parameters::eqHpfFreqID);
    eqLpfFreqParam = apvts.getRawParameterValue (Othrys::Parameters::eqLpfFreqID);
    eqBand65HzParam   = apvts.getRawParameterValue (Othrys::Parameters::eqBand65HzID);
    eqBand125HzParam  = apvts.getRawParameterValue (Othrys::Parameters::eqBand125HzID);
    eqBand250HzParam  = apvts.getRawParameterValue (Othrys::Parameters::eqBand250HzID);
    eqBand500HzParam  = apvts.getRawParameterValue (Othrys::Parameters::eqBand500HzID);
    eqBand1kHzParam   = apvts.getRawParameterValue (Othrys::Parameters::eqBand1kHzID);
    eqBand2kHzParam   = apvts.getRawParameterValue (Othrys::Parameters::eqBand2kHzID);
    eqBand4kHzParam   = apvts.getRawParameterValue (Othrys::Parameters::eqBand4kHzID);
    eqBand8kHzParam   = apvts.getRawParameterValue (Othrys::Parameters::eqBand8kHzID);
    eqBand16kHzParam  = apvts.getRawParameterValue (Othrys::Parameters::eqBand16kHzID);

    // Channel parameters
    inputChannelModeParam = apvts.getRawParameterValue (Othrys::Parameters::inputChannelModeID);
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}

OthrysAudioProcessor::~OthrysAudioProcessor()
{
}

<<<<<<< HEAD
=======
//==============================================================================
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
const juce::String OthrysAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

<<<<<<< HEAD
bool OthrysAudioProcessor::acceptsMidi() const { return false; }
bool OthrysAudioProcessor::producesMidi() const { return false; }
bool OthrysAudioProcessor::isMidiEffect() const { return false; }
double OthrysAudioProcessor::getTailLengthSeconds() const { return 0.0; }
int OthrysAudioProcessor::getNumPrograms() { return 1; }
int OthrysAudioProcessor::getCurrentProgram() { return 0; }
void OthrysAudioProcessor::setCurrentProgram (int index) { juce::ignoreUnused (index); }
const juce::String OthrysAudioProcessor::getProgramName (int index) { juce::ignoreUnused (index); return {}; }
void OthrysAudioProcessor::changeProgramName (int index, const juce::String& newName) { juce::ignoreUnused (index, newName); }

void OthrysAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();

    // 1. Input Stage
    inputGain.prepare (spec);
    inputGain.setRampDurationSeconds (0.02); 

    dcBlocker.prepare (sampleRate, samplesPerBlock, getTotalNumOutputChannels());

    // 2. Oversampling Engine
    oversampler = std::make_unique<juce::dsp::Oversampling<float>> (
        spec.numChannels, 1, juce::dsp::Oversampling<float>::filterHalfBandFIREquiripple, true);
    oversampler->initProcessing (static_cast<size_t> (samplesPerBlock));
    oversampler->reset();

    // 3. Tone Stack
    toneStack.prepare (spec);
    graphicEQ.prepare (spec);

    // 4. Delay & Reverb Stage
    delay.prepare (spec);
    reverb.prepare (spec);
    noiseGate.prepare (spec);

    // 5. Output Stage & Mixer
    outputGain.prepare (spec);
    outputGain.setRampDurationSeconds (0.02);

    limiter.prepare (spec);
    limiter.setThreshold (-0.1f); // Safety ceiling just below 0dBFS
    limiter.setRelease (10.0f);   // Fast 10ms release to prevent pumping

    dryWetMixer.prepare (spec);
    // 20ms smoothing on the mix knob to prevent zipper noise
    dryWetMixer.setMixingRule (juce::dsp::DryWetMixingRule::linear);
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}

void OthrysAudioProcessor::releaseResources()
{
<<<<<<< HEAD
    // Free up any resources here
=======
    oversampler.reset();
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}

bool OthrysAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
<<<<<<< HEAD
    // We support mono or stereo input
    auto inputSet = layouts.getMainInputChannelSet();
    if (inputSet != juce::AudioChannelSet::mono() && inputSet != juce::AudioChannelSet::stereo())
        return false;

    // We support mono or stereo output
    auto outputSet = layouts.getMainOutputChannelSet();
    if (outputSet != juce::AudioChannelSet::mono() && outputSet != juce::AudioChannelSet::stereo())
        return false;

    return true;
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}

void OthrysAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
<<<<<<< HEAD
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

=======
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

<<<<<<< HEAD
    // --- Active Input Channel Selection Mode ---
    auto channelMode = static_cast<int> (inputChannelModeParam->load());
    if (totalNumInputChannels == 1 && totalNumOutputChannels == 2)
    {
        // If input is physically mono, always duplicate to right channel for stereo output
        buffer.copyFrom (1, 0, buffer.getReadPointer (0), buffer.getNumSamples());
    }
    else if (channelMode == 1) // Input 1 only (copy left to right)
    {
        if (totalNumInputChannels >= 2)
            buffer.copyFrom (1, 0, buffer.getReadPointer (0), buffer.getNumSamples());
    }
    else if (channelMode == 2) // Input 2 only (copy right to left)
    {
        if (totalNumInputChannels >= 2)
            buffer.copyFrom (0, 0, buffer.getReadPointer (1), buffer.getNumSamples());
    }

    // If global bypass is ON, bypass the entire processing
    if (bypassParam->load() >= 0.5f)
        return;

    // --- 1. Parameter Fetching ---
    // Read directly from the cached memory addresses (Lock-free and instant)
    auto rawInputGain  = inputGainParam->load();
    auto rawDrive      = driveParam->load();
    auto rawType       = typeParam->load();
    auto rawTone       = toneParam->load();
    auto rawOutputGain = outputGainParam->load();
    auto rawMix        = mixParam->load();
    
    // Update module parameters safely
    inputGain.setGainDecibels (rawInputGain);
    distortion.setType (static_cast<Othrys::DSP::DistortionType> (static_cast<int> (rawType)));
    distortion.setDrive (rawDrive);
    toneStack.setTone (rawTone);
    
    // Reverb and Delay Parameter updates
    delay.updateParameters (delayMixParam->load(), delayTimeParam->load(), delayFeedbackParam->load());
    reverb.updateParameters (
        static_cast<int>(reverbTypeParam->load()),
        reverbMixParam->load(),
        reverbSizeParam->load(),
        reverbDampParam->load(),
        reverbWidthParam->load()
    );

    // Noise Gate updates
    noiseGate.updateParameters (gateThresholdParam->load(), gateAttackParam->load(), gateReleaseParam->load());

    // Graphic EQ updates
    graphicEQ.updateParameters (eqHpfFreqParam->load(), eqLpfFreqParam->load(),
                                eqBand65HzParam->load(),
                                eqBand125HzParam->load(),
                                eqBand250HzParam->load(),
                                eqBand500HzParam->load(),
                                eqBand1kHzParam->load(),
                                eqBand2kHzParam->load(),
                                eqBand4kHzParam->load(),
                                eqBand8kHzParam->load(),
                                eqBand16kHzParam->load());

    outputGain.setGainDecibels (rawOutputGain);
    dryWetMixer.setWetMixProportion (rawMix / 100.0f);

    // --- 2. DSP Processing Pipeline ---
    juce::dsp::AudioBlock<float> audioBlock (buffer);
    juce::dsp::ProcessContextReplacing<float> context (audioBlock);

    // 🔴 CRITICAL: Push a copy of the untouched DRY signal into the mixer memory
    dryWetMixer.pushDrySamples (audioBlock);

    // Stage A: Input Formatting and Sanitization
    inputGain.process (context);
    dcBlocker.process (buffer);
    
    if (gatePowerParam->load() >= 0.5f)
        noiseGate.process (context);
    
    // Stage B: Oversampled Nonlinear Distortion
    if (distortionPowerParam->load() >= 0.5f)
    {
        if (oversampler != nullptr)
        {
            juce::dsp::AudioBlock<float> oversampledBlock = oversampler->processSamplesUp (audioBlock);
            distortion.process (oversampledBlock);
            oversampler->processSamplesDown (audioBlock);
        }
        else
        {
            distortion.process (audioBlock);
        }

        // Stage C: Post-Distortion Tonal Shaping
        toneStack.process (context);
    }
    
    // Stage C1: Graphic EQ (after Tone Stack, before time-based effects)
    if (eqPowerParam->load() >= 0.5f)
        graphicEQ.process (context);

    // Stage C2: Reverb and Delay
    if (delayPowerParam->load() >= 0.5f)
        delay.process (context);

    if (reverbPowerParam->load() >= 0.5f)
        reverb.process (context);

    // Stage D: Output Staging & Safety Limiting
    outputGain.process (context);
    limiter.process (context);

    // 🟢 CRITICAL: Blend the processed WET block with the stored DRY block
    dryWetMixer.mixWetSamples (audioBlock);
}

=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
bool OthrysAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* OthrysAudioProcessor::createEditor()
{
    return new OthrysAudioProcessorEditor (*this);
}

<<<<<<< HEAD
void OthrysAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Save the current parameter state to the DAW project
=======
//==============================================================================
// Clean, Standard JUCE State Management (Replaces the hallucinated lambda loop)
void OthrysAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void OthrysAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
<<<<<<< HEAD
    // Load the parameter state when opening a saved DAW project
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

=======
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

<<<<<<< HEAD
// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new OthrysAudioProcessor();
}

void OthrysAudioProcessor::savePresetToFile (const juce::File& file)
{
    juce::DynamicObject::Ptr jsonObj = new juce::DynamicObject();
    
    auto parameters = getParameters();
    for (auto* param : parameters)
    {
        if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*> (param))
        {
            jsonObj->setProperty (rangedParam->getParameterID(), 
                                  rangedParam->getNormalisableRange().convertFrom0to1 (rangedParam->getValue()));
        }
    }
    
    juce::var jsonVar (jsonObj.get());
    
    juce::FileOutputStream stream (file);
    if (stream.openedOk())
    {
        stream.setPosition (0);
        stream.truncate();
        juce::JSON::writeToStream (stream, jsonVar);
    }
}

void OthrysAudioProcessor::loadPresetFromFile (const juce::File& file)
{
    if (file.getFileExtension().toLowerCase() == ".xml")
    {
        juce::MemoryBlock data;
        if (file.loadFileAsData (data))
        {
            const char* pData = static_cast<const char*> (data.getData());
            if (data.getSize() > 8 && std::memcmp (pData, "henson-x", 8) == 0)
            {
                auto setParamValue = [this](const juce::String& paramID, float value)
                {
                    if (auto* param = apvts.getParameter (paramID))
                    {
                        if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*> (param))
                        {
                            float normalized = rangedParam->getNormalisableRange().convertTo0to1 (value);
                            rangedParam->setValueNotifyingHost (normalized);
                        }
                    }
                };

                auto getFloatVal = [this, &data](const juce::String& key, float defaultVal) -> float
                {
                    auto s = getNeuralDspValue (data, key);
                    if (s.isNotEmpty())
                        return s.getFloatValue();
                    return defaultVal;
                };

                auto getBoolVal = [this, &data](const juce::String& key, bool defaultVal) -> bool
                {
                    auto s = getNeuralDspValue (data, key);
                    if (s.isNotEmpty())
                        return s == "true";
                    return defaultVal;
                };

                float parsedInputGain = getFloatVal ("inputGain", 0.0f);
                float parsedOutputGain = getFloatVal ("outputGain", 0.0f);
                
                bool gateActive = getBoolVal ("gateActive", true);
                float gateThreshold = getFloatVal ("gateThreshold", -80.0f);

                float drive = getFloatVal ("overdriveDrive", 0.0f) * 100.0f;
                float tone = getFloatVal ("overdriveTone", 0.5f) * 2.0f - 1.0f;
                bool overdriveActive = getBoolVal ("overdriveActive", true);

                float delayMix = getFloatVal ("delayMix", 0.0f) * 100.0f;
                float delayTime = getFloatVal ("delayTime", 300.0f);
                float delayFeedback = getFloatVal ("delayFeedback", 0.3f) * 100.0f;
                delayFeedback = std::min (95.0f, delayFeedback);
                bool delayActive = getBoolVal ("delayActive", true);

                float reverbMix = getFloatVal ("reverbMix", 0.0f) * 100.0f;
                float reverbDecay = getFloatVal ("reverbDecay", 0.5f);
                bool reverbActive = getBoolVal ("reverbActive", true);

                std::map<juce::String, float> targets;
                targets[Othrys::Parameters::inputGainID] = parsedInputGain;
                targets[Othrys::Parameters::outputGainID] = parsedOutputGain;
                targets[Othrys::Parameters::gateThresholdID] = gateThreshold;
                targets[Othrys::Parameters::driveID] = drive;
                targets["tone"] = tone;
                targets[Othrys::Parameters::delayMixID] = delayMix;
                targets[Othrys::Parameters::delayTimeID] = delayTime;
                targets[Othrys::Parameters::delayFeedbackID] = delayFeedback;
                targets[Othrys::Parameters::reverbMixID] = reverbMix;
                targets[Othrys::Parameters::reverbSizeID] = reverbDecay;
 
                // Set power states
                targets[Othrys::Parameters::bypassID] = 0.0f;
                targets[Othrys::Parameters::gatePowerID] = gateActive ? 1.0f : 0.0f;
                targets[Othrys::Parameters::distortionPowerID] = overdriveActive ? 1.0f : 0.0f;
                targets[Othrys::Parameters::delayPowerID] = delayActive ? 1.0f : 0.0f;
                targets[Othrys::Parameters::reverbPowerID] = reverbActive ? 1.0f : 0.0f;

                startParameterTransition (targets);
            }
        }
        return;
    }

    // Default to JSON (.rnv)
    juce::var jsonVar = juce::JSON::parse (file);
    auto* jsonObj = jsonVar.getDynamicObject();
    
    if (jsonObj != nullptr)
    {
        std::map<juce::String, float> targets;
        auto parameters = getParameters();
        for (auto* param : parameters)
        {
            if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*> (param))
            {
                auto paramID = rangedParam->getParameterID();
                if (jsonObj->hasProperty (paramID))
                {
                    float value = static_cast<float> (jsonObj->getProperty (paramID));
                    targets[paramID] = value;
                }
            }
        }
        
        startParameterTransition (targets);
    }
}

juce::File OthrysAudioProcessor::getPresetsFolder()
{
    auto dir = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
               .getChildFile ("ProdByRnV")
               .getChildFile ("Othrys")
               .getChildFile ("Presets");
               
    if (!dir.exists())
        dir.createDirectory();
        
    return dir;
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}

juce::Array<juce::File> OthrysAudioProcessor::getPresetFiles()
{
<<<<<<< HEAD
    juce::Array<juce::File> files;
    auto dir = getPresetsFolder();
    dir.findChildFiles (files, juce::File::findFiles, false, "*.rnv;*.xml");
    return files;
}

juce::String OthrysAudioProcessor::getNeuralDspValue (const juce::MemoryBlock& data, const juce::String& key)
{
    auto keyStr = key.toRawUTF8();
    int keyLen = key.length();
    
    const char* pData = static_cast<const char*> (data.getData());
    int dataSize = static_cast<int> (data.getSize());
    
    for (int i = 0; i <= dataSize - keyLen; ++i)
    {
        if (std::memcmp (pData + i, keyStr, static_cast<size_t> (keyLen)) == 0)
        {
            int start = i + keyLen + 4;
            if (start < dataSize)
            {
                juce::MemoryBlock valBlock;
                for (int j = start; j < dataSize; ++j)
                {
                    if (pData[j] == '\0')
                        break;
                    valBlock.append (pData + j, 1);
                }
                valBlock.append ("", 1);
                return juce::String (static_cast<const char*> (valBlock.getData()));
            }
        }
    }
    return {};
}

void OthrysAudioProcessor::startParameterTransition (const std::map<juce::String, float>& targetValues)
{
    activeTransitions.clear();
    
    auto parameters = getParameters();
    for (auto* param : parameters)
    {
        if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*> (param))
        {
            auto paramID = rangedParam->getParameterID();
            auto it = targetValues.find (paramID);
            if (it != targetValues.end())
            {
                // We only animate continuous float parameters, not choices or booleans
                if (dynamic_cast<juce::AudioParameterFloat*> (param) != nullptr)
                {
                    ParameterTransition transition;
                    transition.parameter = rangedParam;
                    // Start value in raw parameter units
                    transition.startValue = rangedParam->getNormalisableRange().convertFrom0to1 (rangedParam->getValue());
                    transition.targetValue = it->second;
                    
                    activeTransitions.push_back (transition);
                }
                else
                {
                    // Bool and Choice parameters change instantly
                    float normalized = rangedParam->getNormalisableRange().convertTo0to1 (it->second);
                    rangedParam->setValueNotifyingHost (normalized);
                }
            }
        }
    }
    
    if (!activeTransitions.empty())
    {
        transitionStep = 0;
        startTimer (16); // ~60 FPS
    }
}

void OthrysAudioProcessor::timerCallback()
{
    transitionStep++;
    float t = static_cast<float> (transitionStep) / transitionDurationSteps;
    
    // Smooth step curve (ease in / ease out)
    float smoothT = t * t * (3.0f - 2.0f * t);
    
    if (t >= 1.0f)
    {
        stopTimer();
        for (auto& transition : activeTransitions)
        {
            float normalized = transition.parameter->getNormalisableRange().convertTo0to1 (transition.targetValue);
            transition.parameter->setValueNotifyingHost (normalized);
        }
        activeTransitions.clear();
    }
    else
    {
        for (auto& transition : activeTransitions)
        {
            float current = transition.startValue + (transition.targetValue - transition.startValue) * smoothT;
            float normalized = transition.parameter->getNormalisableRange().convertTo0to1 (current);
            transition.parameter->setValueNotifyingHost (normalized);
        }
    }
=======
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
>>>>>>> 87efd5a1ff5bb38083b33febfdd358027e8ce7d1
}