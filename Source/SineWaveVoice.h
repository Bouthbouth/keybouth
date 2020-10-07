/*
  ==============================================================================

    SineWaveVoice.h
    Created: 5 May 2020 6:38:29pm
    Author:  alexb

  ==============================================================================
*/

#pragma once


#include <JuceHeader.h>
#include "SineWaveSound.h"

/*******************************************************************************/
/*******************************************************************************/

class SineWaveVoice : public SynthesiserVoice
{
public:

    /********************* PUBLIC METHODS *********************/

    /* SineWaveVoice.
       Class Constructor 
    */
    SineWaveVoice();

    /* SineWaveVoice~.
       Class Destructor.
    */
    ~SineWaveVoice();

    /* canPlaySound.
       Overrode from parent class.
       Must return true if this voice object is capable of playing the given sound. 
       \param   sound, the sound to play
       \return  true if sound is not null 
    */
    bool canPlaySound(SynthesiserSound* sound) override;

    /* startNote.
       Overrode from parent class.
       Called to start a new note.
       \param   midiNoteNumber, current midi note being played
       \param   velocity, velocity of the current note 
       \return  void
    */
    void startNote(int midiNoteNumber, float velocity, SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/) override;

    /* stopNote.
       Overrode from parent class.
       Called to stop a note.
       \param   allowTailOff, 0 or 1 to allow tail off (aka fade-out)
       \return  void
    */
    void stopNote(float /*velocity*/, bool allowTailOff) override;

    /* pitchWheelMoved.
       Overrode from parent class.
       Called when pitch wheel is moved.
       \return void 
    */
    void pitchWheelMoved(int /*newValue*/) override;

    /* controllerMoved.
       Overrode from parent class.
       Called when controller is moved.
       \return void
    */
    void controllerMoved(int /*controllerNumber*/, int /*newValue*/) override;

    /* renderNextBlock.
       overrode from parent class.
       Render next audio block for this voice.
       \param   outputBuffer, output buffer to be filled.
       \param   startSample, index of the starting sample.
       \param   numSamples, number of samples to process.
       \return  void
    */
    void renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

    /* TODO: clarify behavior*/
    using SynthesiserVoice::renderNextBlock;

    /********************* PUBLIC MEMBERS *********************/

    /* ... */

private:

    /********************* PRIVATE METHODS *********************/
    /* ... */

    /********************* PRIVATE MEMBERS *********************/

    double currentAngle     = 0.0;  /* Current Angle of the sinus */
    double angleDelta       = 0.0;  /* Angle increment of the sinus */
    double level            = 0.0;  /* Level of the sinus */
    double tailOff          = 0.0;  /* TailOff of the sinus*/

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineWaveVoice)
};