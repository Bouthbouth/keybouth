/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


/***********************************************************************************/
/******************************* PUBLIC METHODS ************************************/
/***********************************************************************************/

/**************************************************/
/* KeybouthAudioProcessor::KeybouthAudioProcessor */
/**************************************************/

KeybouthAudioProcessor::KeybouthAudioProcessor() : AudioProcessor(getBusesProperties())
{
    /* Reset info to default */
    lastPosInfo.resetToDefault();

    /* Add a sub-tree to store the state of our UI */
    state.state.addChild({ "uiState", { { "width",  400 }, { "height", 200 } }, {} }, -1, nullptr); 

    /* Initialise Synth */
    initialiseSynth();
}

/***********************************************************************************/

/***************************************************/
/* KeybouthAudioProcessor::~KeybouthAudioProcessor */
/***************************************************/
/* Destructor is overrode */


/***********************************************************************************/

/*****************************************/
/* KeybouthAudioProcessor::prepareToPlay */
/*****************************************/

void KeybouthAudioProcessor::prepareToPlay(double newSampleRate, int /*samplesPerBlock*/)
{
    /* pre-playback initialisation */

    /* Update Current playback Sample rate and reset keyboard state*/
    synth.setCurrentPlaybackSampleRate(newSampleRate);
    keyboardState.reset();

    /* Update buffer sizes depending on what precision is used */
    if (isUsingDoublePrecision())
    {
        delayBufferDouble.setSize(2, 12000);
        delayBufferFloat.setSize(1, 1);
    }
    else
    {
        delayBufferFloat.setSize(2, 12000);
        delayBufferDouble.setSize(1, 1);
    }

    /* reset */
    reset();
}


/***********************************************************************************/

/********************************************/
/* KeybouthAudioProcessor::releaseResources */
/********************************************/

void KeybouthAudioProcessor::releaseResources()
{
    /* When playback stops, you can use this as an opportunity to free up any  spare memory, etc. */
    keyboardState.reset();

}


/***********************************************************************************/

/**************************************************/
/* KeybouthAudioProcessor::isBusesLayoutSupported */
/**************************************************/

bool KeybouthAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    /* Only mono/stereo and input/output must have same layout */
    const auto& mainOutput  = layouts.getMainOutputChannelSet();
    const auto& mainInput   = layouts.getMainInputChannelSet();

    /* input and output layout must either be the same or the input must be disabled altogether */
    if (!mainInput.isDisabled() && mainInput != mainOutput)
    {
        return false;
    }

    /* do not allow disabling the main buses */
    if (mainOutput.isDisabled())
    {
        return false;
    }

    /* only allow stereo and mono */
    if (mainOutput.size() > 2)
    {
        return false;
    }

    return true;
}


/***********************************************************************************/

/***********************************************/
/* KeybouthAudioProcessor::processBlock<float> */
/***********************************************/
void KeybouthAudioProcessor::processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    jassert(!isUsingDoublePrecision());
    process(buffer, midiMessages, delayBufferFloat);
}

/***********************************************************************************/

/************************************************/
/* KeybouthAudioProcessor::processBlock<double> */
/************************************************/
void KeybouthAudioProcessor::processBlock(AudioBuffer<double>& buffer, MidiBuffer& midiMessages)
{
    jassert(isUsingDoublePrecision());
    process(buffer, midiMessages, delayBufferDouble);
}


/***********************************************************************************/

/*************************************/
/* KeybouthAudioProcessor::hasEditor */
/*************************************/

bool KeybouthAudioProcessor::hasEditor() const
{
    return true;
}


/***********************************************************************************/

/****************************************/
/* KeybouthAudioProcessor::createEditor */
/****************************************/
AudioProcessorEditor* KeybouthAudioProcessor::createEditor()
{

    return new KeybouthAudioProcessorEditor(*this);

}

/***********************************************************************************/

/***********************************/
/* KeybouthAudioProcessor::getName */
/***********************************/
const String KeybouthAudioProcessor::getName() const               
{ 
    return JucePlugin_Name; 
}


/***********************************************************************************/

/*****************************************/
/* KeybouthAudioProcessor::acceptsMidi() */
/*****************************************/
bool KeybouthAudioProcessor::acceptsMidi() const                   
{ 
    return true; 
}


/***********************************************************************************/

/****************************************/
/* KeybouthAudioProcessor::producesMidi */
/****************************************/
bool KeybouthAudioProcessor::producesMidi() const                  
{ 
    return true; 
}

/***********************************************************************************/

/****************************************/
/* KeybouthAudioProcessor::isMidiEffect */
/****************************************/
bool KeybouthAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

/***********************************************************************************/

/***************************************************/
/* KeybouthAudioProcessor::getTailLengthSeconds()  */
/***************************************************/
double KeybouthAudioProcessor::getTailLengthSeconds() const        
{ 
    return 0.0; 
}

/***********************************************************************************/

/******************************************/
/* KeybouthAudioProcessor::getNumPrograms */
/******************************************/
int KeybouthAudioProcessor::getNumPrograms()                       
{ 
    return 1; 
}


/***********************************************************************************/

/*********************************************/
/* KeybouthAudioProcessor::getCurrentProgram */
/*********************************************/
int KeybouthAudioProcessor::getCurrentProgram()                    
{ 
    return 0; 
}


/***********************************************************************************/

/*********************************************/
/* KeybouthAudioProcessor::setCurrentProgram */
/*********************************************/
void KeybouthAudioProcessor::setCurrentProgram(int)                
{
    return;
}


/***********************************************************************************/

/******************************************/
/* KeybouthAudioProcessor::getProgramName */
/******************************************/
const String KeybouthAudioProcessor::getProgramName(int)           
{ 
    return "Jack"; 
}


/***********************************************************************************/

/*********************************************/
/* KeybouthAudioProcessor::changeProgramName */
/*********************************************/
void KeybouthAudioProcessor::changeProgramName(int, const String&) 
{
    return;
}


/***********************************************************************************/

/***********************************************/
/* KeybouthAudioProcessor::getStateInformation */
/***********************************************/
void KeybouthAudioProcessor::getStateInformation(MemoryBlock& destData)
{

    /* You should use this method to store your parameters in the memory block.
       You could do that either as raw data, or use the XML or ValueTree classes
       as intermediaries to make it easy to save and load complex data.
    */

    // Store an xml representation of our state.
    if (auto xmlState = state.copyState().createXml())
    {
        copyXmlToBinary(*xmlState, destData);
    }
}


/***********************************************************************************/

/***********************************************/
/* KeybouthAudioProcessor::setStateInformation */
/***********************************************/

void KeybouthAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{

    /* You should use this method to restore your parameters from this memory block,
       whose contents will have been created by the getStateInformation() call.
    */

    /* Restore our plug-in's state from the xml representation stored in the above
       method. */
    if (auto xmlState = getXmlFromBinary(data, sizeInBytes))
    {
        state.replaceState(ValueTree::fromXml(*xmlState));
    }
}


/***********************************************************************************/

/*************************************************/
/* KeybouthAudioProcessor::updateTrackProperties */
/*************************************************/
void KeybouthAudioProcessor::updateTrackProperties(const TrackProperties& properties)
{
    trackProperties = properties;

    auto editor     = dynamic_cast<KeybouthAudioProcessorEditor*> (getActiveEditor());
    
    if (editor)
    {
        editor->updateTrackProperties();
    }
        
}


/***********************************************************************************/

/*********************************/
/* KeybouthAudioProcessor::reset */
/*********************************/

void KeybouthAudioProcessor::reset()
{
    /* Use this method as the place to clear any delay lines, buffers, etc, as it
       means there's been a break in the audio's continuity. */
    delayBufferFloat.clear();
    delayBufferDouble.clear();
}




/***********************************************************************************/

/**********************/
/* createPluginFilter */
/**********************/

/* Extern Method 
    This creates new instances of the plugin */
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KeybouthAudioProcessor();
}


/************************************************************************************/
/******************************* PRIVATE METHODS ************************************/
/************************************************************************************/

/***********************************************/
/* KeybouthAudioProcessor::getBusesProperties */
/***********************************************/
AudioProcessor::BusesProperties KeybouthAudioProcessor::getBusesProperties()
{

    return BusesProperties().withInput( "Input" , AudioChannelSet::stereo(), true)
                            .withOutput("Output", AudioChannelSet::stereo(), true);
}

/***********************************************************************************/

/*******************************************/
/* KeybouthAudioProcessor::initialiseSynth */
/*******************************************/
void KeybouthAudioProcessor::initialiseSynth()
{
    /* Declarations */
    int numVoices = 8;
    int i(0);

    /* Add some voices */
    
    for (i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new SineWaveVoice());
    }
    
    /* Give the synth a sound to play */
    synth.addSound(new SineWaveSound());
    
}

/***********************************************************************************/

/*********************************************************/
/* KeybouthAudioProcessor::updateCurrentTimeInfoFromHost */
/*********************************************************/

void KeybouthAudioProcessor::updateCurrentTimeInfoFromHost()
{
    if (auto* ph = getPlayHead())
    {
        AudioPlayHead::CurrentPositionInfo newTime;

        if (ph->getCurrentPosition(newTime))
        {
            lastPosInfo = newTime;  /* Successfully got the current time from the host.. */
            return;
        }
    }

    /* If the host fails to provide the current time, we'll just reset our copy to a default.. */
    lastPosInfo.resetToDefault();
}


/***********************************************************************************/

/***********************************/
/* KeybouthAudioProcessor::process */
/***********************************/
template <typename FloatType>
void KeybouthAudioProcessor::process(AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, AudioBuffer<FloatType>& delayBuffer)
{
    /* Declarations */
    auto gainParamValue     = state.getParameter("gain")->getValue();
    auto delayParamValue    = state.getParameter("delay")->getValue();
    auto numSamples         = buffer.getNumSamples();

    /* 
    In case we have more outputs than inputs, we'll clear any output
    channels that didn't contain input data, (because these aren't
    guaranteed to be empty - they may contain garbage).
    */
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
    {
        buffer.clear(i, 0, numSamples);
    }
    
    /*
    Now pass any incoming midi messages to our keyboard state object, and let it
    add messages to the buffer if the user is clicking on the on-screen keys
    */
    keyboardState.processNextMidiBuffer(midiMessages, 0, numSamples, true);

    /* and now get our synth to process these midi events and generate its output. */
    synth.renderNextBlock(buffer, midiMessages, 0, numSamples);

    /* Apply our delay effect to the new output.. */
    applyDelay(buffer, delayBuffer, delayParamValue);

    /* Apply our gain change to the outgoing data.. */
    applyGain(buffer, delayBuffer, gainParamValue);

    /* Now ask the host for the current time so we can store it to be displayed later... */
    updateCurrentTimeInfoFromHost();
}


/***********************************************************************************/

/*************************************/
/* KeybouthAudioProcessor::applyGain */
/*************************************/
template <typename FloatType>
void KeybouthAudioProcessor::applyGain(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer, float gainLevel)
{
    /* Ignore delayBuffer variable to avoid warning */
    ignoreUnused(delayBuffer);

    /* applyGain to output */
    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        buffer.applyGain(channel, 0, buffer.getNumSamples(), gainLevel);
    }
}


/***********************************************************************************/

/*************************************/
/* KeybouthAudioProcessor::applyDelay */
/*************************************/
template <typename FloatType>
void KeybouthAudioProcessor::applyDelay(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer, float delayLevel)
{
    /* Declarations */
    auto numSamples = buffer.getNumSamples();
    auto delayPos   = 0;

    /* Loop Over output channels */
    for (auto channel = 0; channel < getTotalNumOutputChannels(); ++channel)
    {
        auto channelData        = buffer.getWritePointer(channel);
        auto delayData          = delayBuffer.getWritePointer(jmin(channel, delayBuffer.getNumChannels() - 1));
        delayPos                = delayPosition;

        /* Loop over samples */
        for (auto i = 0; i < numSamples; ++i)
        {
            auto in             = channelData[i];
            channelData[i]      = channelData[i] + delayData[delayPos];
            delayData[delayPos] = (delayData[delayPos] + in) * delayLevel;

            if (++delayPos >= delayBuffer.getNumSamples())
            {
                delayPos        = 0;
            }
        }
    }

    delayPosition = delayPos;
}
