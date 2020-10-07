/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "SineWaveVoice.h"

/***********************************************************************************/
/***********************************************************************************/

class KeybouthAudioProcessor  : public AudioProcessor
{
public:


    /********************* PUBLIC METHODS *********************/

    /* KeybouthAudioProcessor
       Constructor of the class KeybouthAudioProcessor
    */
    KeybouthAudioProcessor();

    /* ~KeybouthAudioProcessor
       Destructor of the class KeybouthAudioProcessor
    */
    ~KeybouthAudioProcessor()  override = default;

    /* prepareToPlay
       Overrode parent class method.
       Any initialisation are made in this method before playing.
       \param   newSampleRate, new sample rate 
       \param   samplesPerBlock, number of samples per processing block
       \return  void 
    */
    void prepareToPlay(double newSampleRate, int /*samplesPerBlock*/) override;

    /* releaseResources
       Overrode parent class method.
       Called when playback is stopped, allow to release memory and so on .
       \return  void
    */
    void releaseResources() override;


    /* isBusesLayoutSupported
       Overrode parent class method.
       Checks if bus layout (aka audio I/O) is supported .
       \param   layouts , I/O layouts
       \return  void
    */
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    /* processBlock
       Overrode parent class method.
       Process next audio block (float flavour)
       \param   buffer, audio buffer to process.
       \param   midiMessages, midi messages received.
       \return  void
    */
    void processBlock(AudioBuffer<float>& buffer, MidiBuffer& midiMessages) override;

    /* processBlock
       Overrode parent class method.
       Process next audio block (doubles flavour)
       \param   buffer, audio buffer to process.
       \param   midiMessages, midi messages received.
       \return  void
    */
    void processBlock(AudioBuffer<double>& buffer, MidiBuffer& midiMessages);

    /* createEditor
       Overrode parent class method.
       Creates a new Editor
       \return AudioProcessorEditor 
    */
    AudioProcessorEditor* createEditor() override;

    /* hasEditor
       Overrode parent class method.
       Returns true.
       \return true
    */
    bool hasEditor() const override;


    /* getName.
       Overrode parent class method.
       Returns plugin name.
       \return plugin name
    */
    const String getName() const override;

    /* acceptsMidi.
       Overrode parent class method.
       Returns whether the plugin accepts midi or not.
       \return true
    */
    bool acceptsMidi() const override;

    /* producesMidi.
       Overrode parent class method.
       Returns whether the plugin produces midi or not.
       \return true
    */
    bool producesMidi() const override;

    /* getTailLengthSeconds.
       Overrode parent class method.
       Returns tails length in seconds.
       \return double value of tail length
    */
    double getTailLengthSeconds() const override;

    /* isMidiEffect 
       Overrode parent class method.
       Returns whether it's a MIDI effect or not.
       \return bool*/
    bool isMidiEffect() const override;

    /* getNumPrograms.
       Overreode parent class method.
       Returns the number of preset programs the processor supports.
       \return 1
    */
    int getNumPrograms() override;

    /* getCurrentProgram.
       Overreode parent class method.
       Returns the number of the currently active program.
       \return 0
    */
    int getCurrentProgram() override;

    /* setCurrentProgram.
       Overreode parent class method.
       Called by the host to change the current program.
       \return void
    */
    void setCurrentProgram (int) override;

    /* getProgramName.
       Overreode parent class method.
       Must return the name of a given program.
       \return program name
    */
    const String getProgramName (int) override;

    /* changeProgramName.
       Overreode parent class method.
       Change program name.
       \return void
    */
    void changeProgramName(int, const String&) override;

    /* getStateInformation 
       Overreode parent class method.
       Get State information.
       \param   destData, MemoryBlock where to collect the state information.
       \return  void
    */
    void getStateInformation (MemoryBlock& destData) override;

    /* setStateInformation.
       Set State information.
       Overreode parent class method.
       \param   data, State Data.
       \parem   sieInBytes, size of the data in Bytes.
       \return  void
    */
    void setStateInformation (const void* data, int sizeInBytes) override;

    /* reset 
       Reset audio buffers.
       \return  void 
    */
    void reset();

    /* updateTrackProperties 
       Update track properties.
       \param   properties, input track properties
       \return  void
    */
    void updateTrackProperties(const TrackProperties& properties) override;

    /********************* PUBLIC MEMBERS *********************/
    /* These properties are public so that our editor component can access them
       A bit of a hacky way to do it, but it's only a demo!
    */

    /* this is kept up to date with the midi messages that arrive, and the UI component
       registers with it so it can represent the incoming messages */
    MidiKeyboardState keyboardState;

    /* this keeps a copy of the last set of time info that was acquired during an audio
       callback - the UI component will read this and display it. */
    AudioPlayHead::CurrentPositionInfo lastPosInfo;

    /* Our plugin's current state */
    AudioProcessorValueTreeState state{ *this,
                                        nullptr,
                                        "state", 
                                        { 
                                            std::make_unique<AudioParameterFloat>("gain", "Gain", NormalisableRange<float>(0.0f, 1.0f), 0.9f), 
                                            std::make_unique<AudioParameterFloat>("delay", "Delay Feedback", NormalisableRange<float>(0.0f, 1.0f), 0.5f) 
                                        } 
                                       };

    /* Current track colour and name */
    TrackProperties trackProperties;


private:


    /********************* PRIVATE  METHODS *********************/
   
    /* getBusesProperties
       Return buses properties
       \return buses properties
    */
    static BusesProperties getBusesProperties();

    /* initialiseSynth.
       Initialises Synth with voices and sounds
       \return void
    */
    void initialiseSynth();

    /* updateCurrentTimeInfoFromHost
       Return current time from the host
       \return time of the host
    */
    void updateCurrentTimeInfoFromHost();

    /* process
       Process next incoming audio block
       \param   buffer, input audio buffer
       \param   midiMessages, received midi Messages
       \param   delayBuffer, audio delay buffer 
    */
    template <typename FloatType>
    void process(AudioBuffer<FloatType>& buffer, MidiBuffer& midiMessages, AudioBuffer<FloatType>& delayBuffer);

    /* applyGain
       Apply gain to next incoming audio block
       \param   buffer, input audio buffer
       \param   midiMessages, received midi Messages
       \param   delayBuffer, audio delay buffer 
    */
    template <typename FloatType>
    void applyGain(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer, float gainLevel);


    /* applyDelay
       Apply delay to next incoming audio block
       \param   buffer, input audio buffer
       \param   midiMessages, received midi Messages
       \param   delayBuffer, audio delay buffer
    */
    template <typename FloatType>
    void applyDelay(AudioBuffer<FloatType>& buffer, AudioBuffer<FloatType>& delayBuffer, float delayLevel);


    /********************* PRIVATE  MEMBERS *********************/

    /* Audio Buffers */
    AudioBuffer<float> delayBufferFloat;
    AudioBuffer<double> delayBufferDouble;

    /* Used in order to apply delay */
    int delayPosition = 0;

    /* Synthesiser */
    Synthesiser synth;

    /***********************************************************************************/
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeybouthAudioProcessor)
};
