/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <vector>
#include <memory>

//==============================================================================
IIRfilter_LPAudioProcessor::IIRfilter_LPAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
       mApvts(*this, nullptr, "Parameters", createParameterLayout())
#endif
{
    
}

IIRfilter_LPAudioProcessor::~IIRfilter_LPAudioProcessor()
{
}

//==============================================================================
const juce::String IIRfilter_LPAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool IIRfilter_LPAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool IIRfilter_LPAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool IIRfilter_LPAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double IIRfilter_LPAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int IIRfilter_LPAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int IIRfilter_LPAudioProcessor::getCurrentProgram()
{
    return 0;
}

void IIRfilter_LPAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String IIRfilter_LPAudioProcessor::getProgramName (int index)
{
    return {};
}

void IIRfilter_LPAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void IIRfilter_LPAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
   LPfilter.prepare(sampleRate);
}

void IIRfilter_LPAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool IIRfilter_LPAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void IIRfilter_LPAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    const float cuttoff = *mApvts.getRawParameterValue("Cutoff");
    const float q = *mApvts.getRawParameterValue("Q factor");
    
    LPfilter.updateCoefficients(cuttoff, q);
    
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* outChannelData = buffer.getWritePointer(channel);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            outChannelData[i] = LPfilter.process(outChannelData[i]);
        }
    }
}

//==============================================================================
bool IIRfilter_LPAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* IIRfilter_LPAudioProcessor::createEditor()
{
    return new IIRfilter_LPAudioProcessorEditor (*this);
}

//==============================================================================
void IIRfilter_LPAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, oㅌr use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void IIRfilter_LPAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new IIRfilter_LPAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout IIRfilter_LPAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Cutoff", 1},
                                                                 "Cutoff Frequency",
                                                                 juce::NormalisableRange<float>
                                                                 (20.0f, 20000.0f, 1.0f),
                                                                 1000.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(juce::ParameterID{"Q factor", 1},
                                                                 "Q factor",
                                                                 juce::NormalisableRange<float>
                                                                 (0.1f, 10.0f, 0.1f),
                                                                 0.707f));
    
    return {params.begin(), params.end()};
    
}

juce::AudioProcessorValueTreeState& IIRfilter_LPAudioProcessor::getApvts()
{
    return mApvts;
}
