/*
  ==============================================================================

    SineWaveSound.h
    Created: 5 May 2020 6:37:45pm
    Author:  alexb

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


/*******************************************************************************/
/** A demo synth sound that's just a basic sine wave.. */
class SineWaveSound : public SynthesiserSound
{
public:
    /********************* PUBLIC METHODS *********************/

    /* SineWaveSound.
       Class Constructor
    */
    SineWaveSound();

    /* ~SineWaveSound.
       Class Destructor.
    */
    ~SineWaveSound();

    /* appliesToNote.
       Overrode from parent class.
       Returns true if this sound should be played when a given midi note is pressed.
       \return  true
    */
    bool appliesToNote(int /*midiNoteNumber*/) override;

    /* appliesToChannel
       Overrode from parent class.
       Returns true if the sound should be triggered by midi events on a given channel.
       \return true
    */
    bool appliesToChannel(int /*midiChannel*/) override;

    /********************* PUBLIC MEMBERS *********************/

    /* ... */

private:

    /********************* PRIVATE METHODS *********************/

    /* ... */

    /********************* PRIVATE MEMBERS *********************/

    /*...*/

    /***************************************************************************/
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SineWaveSound)
};