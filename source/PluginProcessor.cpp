#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
PluginProcessor::PluginProcessor()
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
{
    notesList.add("8");
    notesList.add("6");
    notesList.add("4");
    notesList.add("3");
    notesList.add("2");
    notesList.add("1.5");
    notesList.add("1");
    notesList.add("3/4");
    notesList.add("1/2");
    notesList.add("3/8");
    notesList.add("1/3");
    notesList.add("5/16");
    notesList.add("1/4");
    notesList.add("3/16");
    notesList.add("1/6");
    notesList.add("1/8");
    notesList.add("1/12");
    notesList.add("1/16");
    notesList.add("1/24");
    notesList.add("1/32");
    notesList.add("1/48");
    notesList.add("1/64");
}

PluginProcessor::~PluginProcessor()
{
}

//==============================================================================
const juce::String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool PluginProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
    juce::ignoreUnused (index);
}

const juce::String PluginProcessor::getProgramName (int index)
{
    juce::ignoreUnused (index);
    return {};
}

void PluginProcessor::changeProgramName (int index, const juce::String& newName)
{
    juce::ignoreUnused (index, newName);
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = (juce::uint32) samplesPerBlock;
    spec.numChannels = (juce::uint32) getTotalNumInputChannels();
    spec.sampleRate = sampleRate;

    delayValue.reset(sampleRate, 0.01);

    chorusDSPObj.prepare(spec);
}

void PluginProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void PluginProcessor::processBlock (juce::AudioBuffer<float>& buffer,
                                              juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused (midiMessages);

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

    }
    delayValue.setTargetValue(*apvts.getRawParameterValue("delay"));

    updateChorusParams();

    auto block = juce::dsp::AudioBlock<float>(buffer);
    auto context = juce::dsp::ProcessContextReplacing<float>(block);

    chorusDSPObj.process(context);
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new PluginEditor (*this);
//    return new juce::GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused (destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused (data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout PluginProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    juce::StringArray notes;
    notes.add("8");
    notes.add("6");
    notes.add("4");
    notes.add("3");
    notes.add("2");
    notes.add("1.5");
    notes.add("1");
    notes.add("3/4");
    notes.add("1/2");
    notes.add("3/8");
    notes.add("1/3");
    notes.add("5/16");
    notes.add("1/4");
    notes.add("3/16");
    notes.add("1/6");
    notes.add("1/8");
    notes.add("1/12");
    notes.add("1/16");
    notes.add("1/24");
    notes.add("1/32");
    notes.add("1/48");
    notes.add("1/64");

    layout.add(std::make_unique<juce::AudioParameterFloat>((juce::ParameterID{"delay", 1}),
        "delay",
        juce::NormalisableRange<float>(1.f, 20.f, 0.01f, 1.f),
            1.f));
//    Sets the centre delay in milliseconds of the chorus delay line modulation. must be between 1 and 100 ms.

    layout.add(std::make_unique<juce::AudioParameterFloat>((juce::ParameterID{"depth", 1}),
        "depth",
        juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
        0.f));
//    Sets the volume of the LFO modulating the chorus delay line (between 0 and 1).

    layout.add(std::make_unique<juce::AudioParameterFloat>((juce::ParameterID{"feedback", 1}),
        "feedback",
        juce::NormalisableRange<float>(-1.f, 1.f, 0.01f, 1.f),
        0.f));
//    Sets the feedback volume (between -1 and 1) of the chorus delay line.
    //    Negative values can be used to get specific chorus sounds.

    layout.add(std::make_unique<juce::AudioParameterFloat>((juce::ParameterID{"rate", 1}),
        "rate",
        juce::NormalisableRange<float>(0.01f, 40.f, 0.01f, 1.f),
        0.f));
//    rate in Hz <100

    layout.add(std::make_unique<juce::AudioParameterFloat>((juce::ParameterID{"mix", 1}),
        "mix",
        juce::NormalisableRange<float>(0.f, 1.f, 0.01f, 1.f),
        0.5f));

    layout.add(std::make_unique<juce::AudioParameterChoice>((juce::ParameterID{"rateNotes", 1}),
            "rateNotes",
            notes,
            6));

    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("noteRateButton", 1), "noteRateButton", true));
    layout.add(std::make_unique<juce::AudioParameterBool>(juce::ParameterID("hzRateButton", 1), "hzRateButton",  false));

    return layout;
}

void PluginProcessor::updateChorusParams()
{
    chorusDSPObj.setCentreDelay(delayValue.getNextValue());
    chorusDSPObj.setDepth(*apvts.getRawParameterValue("depth"));
    chorusDSPObj.setFeedback(*apvts.getRawParameterValue("feedback"));
    chorusDSPObj.setMix(*apvts.getRawParameterValue("mix"));
    if(notes)
    {
        chorusDSPObj.setRate (convertNoteToHz());
    }
    else
    {
        chorusDSPObj.setRate (*apvts.getRawParameterValue ("rate"));
    }

}

float PluginProcessor::convertNoteToHz()
{
    float oneCyclePerBeatInHz = 0;
    if(cpi.bpm > 0)
    {
        oneCyclePerBeatInHz = 60.f/ (float)cpi.bpm;
    }

    float noteValue = 0;

    switch (static_cast<int> (*apvts.getRawParameterValue("rateNotes")))
    {
        case 0:
            noteValue = 8.f;
            break;
        case 1:
            noteValue = 6.f;
            break;
        case 2:
            noteValue = 4.f;
            break;
        case 3:
            noteValue = 3.f;
            break;
        case 4:
            noteValue = 2.f;
            break;
        case 5:
            noteValue = 1.5f;
            break;
        case 6:
            noteValue = 1.f;
            break;
        case 7:
            noteValue = 3.f/4.f;
            break;
        case 8:
            noteValue = 0.5f;
            break;
        case 9:
            noteValue = 3.f/8.f;
            break;
        case 10:
            noteValue = 1.f/3.f;
            break;
        case 11:
            noteValue = 5.f/16.f;
            break;
        case 12:
            noteValue = 0.25f;
            break;
        case 13:
            noteValue = 3.f/16.f;
            break;
        case 14:
            noteValue = 1.f/6.f;
            break;
        case 15:
            noteValue = 1.f/8.f;
            break;
        case 16:
            noteValue = 1.f/12.f;
            break;
        case 17:
            noteValue = 1.f/16.f;
            break;
        case 18:
            noteValue = 1.f/24.f;
            break;
        case 19:
            noteValue = 1.f/32.f;
            break;
        case 20:
            noteValue = 1.f/48.f;
            break;
        case 21:
            noteValue = 1.f/64.f;
            break;
    }

    return oneCyclePerBeatInHz/noteValue;
}


