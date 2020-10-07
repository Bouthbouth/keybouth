/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

/***********************************************************************************/
/***********************************************************************************/

class KeybouthAudioProcessorEditor  :  public AudioProcessorEditor,
                                       private Timer,
                                       private Value::Listener
{
public:

    /********************* PUBLIC METHODS *********************/

    /* KeybouthAudioProcessorEditor 
       Constructor of the class KeybouthAudioProcessorEditor 
    */
    KeybouthAudioProcessorEditor(KeybouthAudioProcessor& owner);

    /* ~KeybouthAudioProcessorEditor 
       Destructor of the class KeybouthAudioProcessorEditor
    */
    ~KeybouthAudioProcessorEditor() override;

    /* paint 
       Method called to paint our editor, this is where we can set our editor's colours.
       Overrode from parent class.
       \input   : g, a graphic context used for drawing a component or an image
       \return  : void
    */
    void paint (Graphics& g) override;

    /* resized
       Method to lay out child components in the UI.
       Overrode from parent class.
       \return  : void
    */
    void resized() override;
    void timerCallback() override;


    /* updateTrackProperties. 
       Informs the AudioProcessor that track properties such as the track's name or colour has been changed.
       If you are hosting this AudioProcessor then use this method to inform the AudioProcessor about 
       which track the AudioProcessor is loaded on. This method may only be called on the message thread.
       If you are implementing an AudioProcessor then you can override this callback to do something useful 
       with the track properties such as changing the colour of your AudioProcessor's editor. 
    */
    void updateTrackProperties();

    /* hostMIDIControllerIsAvailable
       Called to indicate if a host is providing a MIDI controller when the host
       reconfigures its layout.
    
       Use this as an opportunity to hide or display your own onscreen keyboard or
       other input component.
    
       Currently only AUv3 plug-ins compiled for MacOS 10.13 or iOS 11.0 (or later)
       support this functionality.
       
       Overrode from parent class.

       \input   : controllerIsAvailable, boolean that specifies whether an host's controller is available or not.
       \return  : void.
    */
    void hostMIDIControllerIsAvailable(bool controllerIsAvailable) override;


    /********************* PUBLIC MEMBERS *********************/

    /* ... */

private:

    /********************* PRIVATE METHODS *********************/

    /* getProcessor 
       Returns the processor instance. 
    */
    KeybouthAudioProcessor& getProcessor() const;

    /* updateTimecodeDisplay 
       Update the time code display in the header of the plugin.
       \input   : pos, current position info of the play head.
       \return  : void
    */
    void updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos);

    /* timeToTimecodeString.
       Quick and dirty formating function.
       \input   seconds, time in seconds
       \return  string,  formated string 
    */
    static String timeToTimecodeString(double seconds);

    /* quarterNotePositionToBarsBeatsString
       Formatting function that converts quarter notes to bars beats string.
       \input   quarterNotes, quarter notes.
       \input   numerator   , numerator of the time signature.
       \input   denominator , denominator of the time signature.
       \return  formatted string.
    */
    static String quarterNotePositionToBarsBeatsString(double quarterNotes, int numerator, int denominator);

    /* valueChanged.
       called when the stored window size changes
       \return void
    */
    void valueChanged(Value&) override;

    /********************* PRIVATE MEMBERS *********************/

    /*This reference is provided as a quick way for your editor to
      access the processor object that created it. */
    KeybouthAudioProcessor& processor;
    
    /* midi Keyboard */
    MidiKeyboardComponent midiKeyboard;

    /* Labels */
    Label timecodeDisplayLabel;
    Label gainLabel{ {}, "Throughput level:" };
    Label delayLabel{ {}, "Delay:" };

    /* Sliders and associated functions */
    Slider gainSlider;
    Slider delaySlider;
    AudioProcessorValueTreeState::SliderAttachment gainAttachment;
    AudioProcessorValueTreeState::SliderAttachment delayAttachment;

    /* Background colour */
    Colour backgroundColour;

    /* these are used to persist the UI's size - the values are stored along with the
       filter's other parameters, and the UI component will update them when it gets
       resized. */
    Value lastUIWidth;
    Value lastUIHeight;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeybouthAudioProcessorEditor)
};
