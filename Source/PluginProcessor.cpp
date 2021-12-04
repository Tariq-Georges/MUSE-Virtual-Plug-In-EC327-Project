/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FiveBandEQAudioProcessor::FiveBandEQAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
}

FiveBandEQAudioProcessor::~FiveBandEQAudioProcessor()
{
}

//==============================================================================
const juce::String FiveBandEQAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FiveBandEQAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FiveBandEQAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FiveBandEQAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FiveBandEQAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FiveBandEQAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FiveBandEQAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FiveBandEQAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FiveBandEQAudioProcessor::getProgramName (int index)
{
    return {};
}

void FiveBandEQAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FiveBandEQAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void FiveBandEQAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FiveBandEQAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FiveBandEQAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
    }
}

//==============================================================================
bool FiveBandEQAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FiveBandEQAudioProcessor::createEditor()
{
//    return new FiveBandEQAudioProcessorEditor (*this);
    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void FiveBandEQAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FiveBandEQAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

juce::AudioProcessorValueTreeState::ParameterLayout FiveBandEQAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("LowCut Freq",
                                                           "LowCut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 20.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("HighCut Freq",
                                                           "HighCut Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 20000.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak1 Freq",
                                                           "Peak1 Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 350.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak1 Gain",
                                                           "Peak1 Gain",
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak2 Freq",
                                                           "Peak2 Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 2000.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak2 Gain",
                                                           "Peak2 Gain",
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak3 Freq",
                                                           "Peak3 Freq",
                                                           juce::NormalisableRange<float>(20.f, 20000.f, 1.f, 1.f), 750.f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak3 Gain",
                                                           "Peak3 Gain",
                                                           juce::NormalisableRange<float>(-24.f, 24.f, 0.5f, 1.f), 0.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak1 Quality",
                                                           "Peak1 Quality",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f), 1.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak2 Quality",
                                                           "Peak2 Quality",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f), 1.0f));
    
    layout.add(std::make_unique<juce::AudioParameterFloat>("Peak3 Quality",
                                                           "Peak3 Quality",
                                                           juce::NormalisableRange<float>(0.1f, 10.f, 0.05f, 1.f), 1.0f));
    
    
    juce::StringArray stringArray;
    for(int i = 0; i < 4; i++)
    {
        juce::String str;
        str << (12 + i*12);
        str << " db/Oct";
        stringArray.add(str);
    }
    
    layout.add(std::make_unique<juce::AudioParameterChoice>("LowCut Slope", "LowCut Slope", stringArray, 0));
    layout.add(std::make_unique<juce::AudioParameterChoice>("HighCut Slope", "HighCut Slope", stringArray, 0));
    // these two layout adds create a drop down to choose the slope for the plugin to change how intense the slope of the cutoff frequencies are
    
    // skew factor allows slider to move differently *more slide for lower end than higher end*
    
    return layout;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FiveBandEQAudioProcessor();
}