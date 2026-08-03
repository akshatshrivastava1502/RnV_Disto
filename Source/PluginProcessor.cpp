#include "PluginProcessor.h"

// --- Add this explicit include ---
#include "PluginEditor.h"
// ---------------------------------

// ... rest of your implementation remains the same
RnVDistoAudioProcessor::RnVDistoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                     .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     ),
       // Initialize the APVTS with our custom layout
       apvts (*this, nullptr, "Parameters", RnVDisto::Parameters::createParameterLayout())
#endif
{
    // Cache the atomic pointers here to avoid string lookups on the audio thread
    inputGainParam  = apvts.getRawParameterValue (RnVDisto::Parameters::inputGainID);
    driveParam      = apvts.getRawParameterValue (RnVDisto::Parameters::driveID);
    typeParam       = apvts.getRawParameterValue (RnVDisto::Parameters::typeID);
    toneParam       = apvts.getRawParameterValue ("tone");
    outputGainParam = apvts.getRawParameterValue (RnVDisto::Parameters::outputGainID);
    mixParam        = apvts.getRawParameterValue (RnVDisto::Parameters::mixID);

    // Reverb parameters
    reverbTypeParam  = apvts.getRawParameterValue (RnVDisto::Parameters::reverbTypeID);
    reverbMixParam   = apvts.getRawParameterValue (RnVDisto::Parameters::reverbMixID);
    reverbSizeParam  = apvts.getRawParameterValue (RnVDisto::Parameters::reverbSizeID);
    reverbDampParam  = apvts.getRawParameterValue (RnVDisto::Parameters::reverbDampID);
    reverbWidthParam = apvts.getRawParameterValue (RnVDisto::Parameters::reverbWidthID);

    // Delay parameters
    delayMixParam      = apvts.getRawParameterValue (RnVDisto::Parameters::delayMixID);
    delayTimeParam     = apvts.getRawParameterValue (RnVDisto::Parameters::delayTimeID);
    delayFeedbackParam = apvts.getRawParameterValue (RnVDisto::Parameters::delayFeedbackID);

    // Noise Gate parameters
    gateThresholdParam = apvts.getRawParameterValue (RnVDisto::Parameters::gateThresholdID);
    gateAttackParam    = apvts.getRawParameterValue (RnVDisto::Parameters::gateAttackID);
    gateReleaseParam   = apvts.getRawParameterValue (RnVDisto::Parameters::gateReleaseID);

    // Power parameters
    bypassParam          = apvts.getRawParameterValue (RnVDisto::Parameters::bypassID);
    distortionPowerParam = apvts.getRawParameterValue (RnVDisto::Parameters::distortionPowerID);
    reverbPowerParam     = apvts.getRawParameterValue (RnVDisto::Parameters::reverbPowerID);
    delayPowerParam      = apvts.getRawParameterValue (RnVDisto::Parameters::delayPowerID);
    gatePowerParam       = apvts.getRawParameterValue (RnVDisto::Parameters::gatePowerID);

    // Channel parameters
    inputChannelModeParam = apvts.getRawParameterValue (RnVDisto::Parameters::inputChannelModeID);
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
}

void RnVDistoAudioProcessor::releaseResources()
{
    // Free up any resources here
}

bool RnVDistoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // We support mono or stereo input
    auto inputSet = layouts.getMainInputChannelSet();
    if (inputSet != juce::AudioChannelSet::mono() && inputSet != juce::AudioChannelSet::stereo())
        return false;

    // We support mono or stereo output
    auto outputSet = layouts.getMainOutputChannelSet();
    if (outputSet != juce::AudioChannelSet::mono() && outputSet != juce::AudioChannelSet::stereo())
        return false;

    return true;
}

void RnVDistoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    juce::ignoreUnused (midiMessages);

    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

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
    distortion.setType (static_cast<RnVDisto::DSP::DistortionType> (static_cast<int> (rawType)));
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
    // Save the current parameter state to the DAW project
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void RnVDistoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Load the parameter state when opening a saved DAW project
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
            apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

// This creates new instances of the plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new RnVDistoAudioProcessor();
}

void RnVDistoAudioProcessor::savePresetToFile (const juce::File& file)
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

void RnVDistoAudioProcessor::loadPresetFromFile (const juce::File& file)
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

                setParamValue (RnVDisto::Parameters::inputGainID, parsedInputGain);
                setParamValue (RnVDisto::Parameters::outputGainID, parsedOutputGain);
                setParamValue (RnVDisto::Parameters::gateThresholdID, gateThreshold);
                setParamValue (RnVDisto::Parameters::driveID, drive);
                setParamValue ("tone", tone);
                setParamValue (RnVDisto::Parameters::delayMixID, delayMix);
                setParamValue (RnVDisto::Parameters::delayTimeID, delayTime);
                setParamValue (RnVDisto::Parameters::delayFeedbackID, delayFeedback);
                setParamValue (RnVDisto::Parameters::reverbMixID, reverbMix);
                setParamValue (RnVDisto::Parameters::reverbSizeID, reverbDecay);

                // Set power states
                setParamValue (RnVDisto::Parameters::bypassID, 0.0f);
                setParamValue (RnVDisto::Parameters::gatePowerID, gateActive ? 1.0f : 0.0f);
                setParamValue (RnVDisto::Parameters::distortionPowerID, overdriveActive ? 1.0f : 0.0f);
                setParamValue (RnVDisto::Parameters::delayPowerID, delayActive ? 1.0f : 0.0f);
                setParamValue (RnVDisto::Parameters::reverbPowerID, reverbActive ? 1.0f : 0.0f);
            }
        }
        return;
    }

    // Default to JSON (.rnv)
    juce::var jsonVar = juce::JSON::parse (file);
    auto* jsonObj = jsonVar.getDynamicObject();
    
    if (jsonObj != nullptr)
    {
        auto parameters = getParameters();
        for (auto* param : parameters)
        {
            if (auto* rangedParam = dynamic_cast<juce::RangedAudioParameter*> (param))
            {
                auto paramID = rangedParam->getParameterID();
                if (jsonObj->hasProperty (paramID))
                {
                    float value = static_cast<float> (jsonObj->getProperty (paramID));
                    float normalized = rangedParam->getNormalisableRange().convertTo0to1 (value);
                    rangedParam->setValueNotifyingHost (normalized);
                }
            }
        }
    }
}

juce::File RnVDistoAudioProcessor::getPresetsFolder()
{
    auto dir = juce::File::getSpecialLocation (juce::File::userDocumentsDirectory)
               .getChildFile ("ProdByRnV")
               .getChildFile ("RnV Disto")
               .getChildFile ("Presets");
               
    if (!dir.exists())
        dir.createDirectory();
        
    return dir;
}

juce::Array<juce::File> RnVDistoAudioProcessor::getPresetFiles()
{
    juce::Array<juce::File> files;
    auto dir = getPresetsFolder();
    dir.findChildFiles (files, juce::File::findFiles, false, "*.rnv;*.xml");
    return files;
}

juce::String RnVDistoAudioProcessor::getNeuralDspValue (const juce::MemoryBlock& data, const juce::String& key)
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