/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

/***********************************************************************************/
/******************************* PUBLIC METHODS ************************************/
/***********************************************************************************/

/**************************************************************/
/* KeybouthAudioProcessorEditor::KeybouthAudioProcessorEditor */
/**************************************************************/

KeybouthAudioProcessorEditor::KeybouthAudioProcessorEditor(KeybouthAudioProcessor& owner) : AudioProcessorEditor(&owner),
                                                                                            processor(owner),
                                                                                            midiKeyboard(owner.keyboardState,MidiKeyboardComponent::horizontalKeyboard),
                                                                                            gainAttachment(owner.state, "gain", gainSlider),
                                                                                            delayAttachment(owner.state, "delay", delaySlider)
{

    /* Add Gain Slider and its label */
    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(Slider::Rotary);
    gainLabel.attachToComponent(&gainSlider, false);
    gainLabel.setFont(Font(11.0f));
    
    /* Add Delay Slider  and its label */
    addAndMakeVisible(delaySlider);
    delaySlider.setSliderStyle(Slider::Rotary);
    delayLabel.attachToComponent(&delaySlider, false);
    delayLabel.setFont(Font(11.0f));

    /* add the midi keyboard component */
    addAndMakeVisible(midiKeyboard);

    /* add a label that will display the current timecode and status.. */
    addAndMakeVisible(timecodeDisplayLabel);
    timecodeDisplayLabel.setFont(Font(Font::getDefaultMonospacedFontName(), 15.0f, Font::plain));

    /* set resize limits for this plug-in */
    setResizeLimits(400, 200, 1024, 700);

    lastUIWidth.referTo(owner.state.state.getChildWithName("uiState").getPropertyAsValue("width", nullptr));
    lastUIHeight.referTo(owner.state.state.getChildWithName("uiState").getPropertyAsValue("height", nullptr));

    /* set our component's initial size to be the last one that was stored in the filter's settings */
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());

    lastUIWidth.addListener(this);
    lastUIHeight.addListener(this);

    /* Update Track Properties */
    updateTrackProperties();


    /* start a timer which will keep our timecode display updated */
    startTimerHz(30);
}


/***********************************************************************************/

/*****************************************************************/
/* KeybouthAudioProcessorEditor::~KeybouthAudioProcessorEditor() */
/*****************************************************************/

KeybouthAudioProcessorEditor::~KeybouthAudioProcessorEditor()
{
}

/***********************************************************************************/

/***************************************/
/* KeybouthAudioProcessorEditor::paint */
/***************************************/

void KeybouthAudioProcessorEditor::paint (Graphics& g)
{
    g.setColour(backgroundColour);
    g.fillAll();
}

/***********************************************************************************/

/*****************************************/
/* KeybouthAudioProcessorEditor::resized */
/*****************************************/

void KeybouthAudioProcessorEditor::resized()
{
    // Laying out the child components */

    /* Declarations */
    Rectangle<int> r            = getLocalBounds().reduced(8);
    Rectangle<int> sliderArea;

    /* Laying out time code labe */
    timecodeDisplayLabel        .setBounds(r.removeFromTop(26));

    /* Laying out midi Keyboard */
    midiKeyboard                .setBounds(r.removeFromBottom(70));

    r                           .removeFromTop(20);
    sliderArea                  = r.removeFromTop(60);

    /* Gain Slider and Delay Slider */
    gainSlider                  .setBounds(sliderArea.removeFromLeft(jmin(180, sliderArea.getWidth() / 2)));
    delaySlider                 .setBounds(sliderArea.removeFromLeft(jmin(180, sliderArea.getWidth())));

    /* Update Width and Height in memory */
    lastUIWidth                 = getWidth();
    lastUIHeight                = getHeight();
}

/***********************************************************************************/

/***********************************************/
/* KeybouthAudioProcessorEditor::timerCallback */
/***********************************************/

void KeybouthAudioProcessorEditor::timerCallback()
{
    updateTimecodeDisplay(getProcessor().lastPosInfo);
}

/***********************************************************************************/

/*******************************************************/
/* KeybouthAudioProcessorEditor::updateTrackProperties */
/*******************************************************/
void KeybouthAudioProcessorEditor::updateTrackProperties()
{
    /* Basically updating the background colour at the moment.
       This function can also do other useful stuff */
    auto trackColour    = getProcessor().trackProperties.colour;
    auto& lf            = getLookAndFeel();


    if (trackColour == Colour())
    {
        backgroundColour    = lf.findColour(ResizableWindow::backgroundColourId);
    }
    else
    {
        backgroundColour    = trackColour.withAlpha(1.0f).withBrightness(0.266f);
    }
    
    repaint();
}

/***********************************************************************************/

/***************************************************************/
/* KeybouthAudioProcessorEditor::hostMIDIControllerIsAvailable */
/***************************************************************/

void KeybouthAudioProcessorEditor::hostMIDIControllerIsAvailable(bool controllerIsAvailable)
{
    midiKeyboard.setVisible(!controllerIsAvailable);
}

/************************************************************************************/
/******************************* PRIVATE METHODS ************************************/
/************************************************************************************/

/**********************************************/
/* KeybouthAudioProcessorEditor::getProcessor */
/**********************************************/

KeybouthAudioProcessor& KeybouthAudioProcessorEditor::getProcessor() const
{
    return static_cast<KeybouthAudioProcessor&> (processor);
}


/***********************************************************************************/

/*******************************************************/
/* KeybouthAudioProcessorEditor::updateTimecodeDisplay */
/*******************************************************/
void KeybouthAudioProcessorEditor::updateTimecodeDisplay(AudioPlayHead::CurrentPositionInfo pos)
{
    MemoryOutputStream displayText;

    /* Format some information in a string */
    displayText << "["                  << SystemStats::getJUCEVersion() << "]   "
                << String(pos.bpm, 2)   << " bpm, "
                << pos.timeSigNumerator << '/' << pos.timeSigDenominator
                << "  -  "              << timeToTimecodeString(pos.timeInSeconds)
                << "  -  "              << quarterNotePositionToBarsBeatsString(pos.ppqPosition, pos.timeSigNumerator, pos.timeSigDenominator);

    if (pos.isRecording)
    {
        displayText << "  (recording)";
    }
    else if (pos.isPlaying)
    {
        displayText << "  (playing)";
    }

    timecodeDisplayLabel.setText(displayText.toString(), dontSendNotification);
}

/***********************************************************************************/

/******************************************************/
/* KeybouthAudioProcessorEditor::timeToTimecodeString */
/******************************************************/
String KeybouthAudioProcessorEditor::timeToTimecodeString(double seconds)
{
    int millisecs       = roundToInt(seconds * 1000.0);
    int absMillisecs    = std::abs(millisecs);
    String str;

    str     = String::formatted("%02d:%02d:%02d.%03d", millisecs / 3600000, (absMillisecs / 60000) % 60, (absMillisecs / 1000) % 60, absMillisecs % 1000);

    return str;
}

/***********************************************************************************/

/**********************************************************************/
/* KeybouthAudioProcessorEditor::quarterNotePositionToBarsBeatsString */
/**********************************************************************/
String KeybouthAudioProcessorEditor::quarterNotePositionToBarsBeatsString(double quarterNotes, int numerator, int denominator)
{
    /* Default */
    if (numerator == 0 || denominator == 0)
    {
        return "1|1|000";
    }

    auto quarterNotesPerBar = (numerator * 4 / denominator);
    auto beats              = (fmod(quarterNotes, quarterNotesPerBar) / quarterNotesPerBar) * numerator;

    auto bar                = ((int)quarterNotes) / quarterNotesPerBar + 1;
    auto beat               = ((int)beats) + 1;
    auto ticks              = ((int)(fmod(beats, 1.0) * 960.0 + 0.5));

    return String::formatted("%d|%d|%03d", bar, beat, ticks);
}

/***********************************************************************************/

/**********************************************/
/* KeybouthAudioProcessorEditor::valueChanged */
/**********************************************/
void KeybouthAudioProcessorEditor::valueChanged(Value&)
{
    setSize(lastUIWidth.getValue(), lastUIHeight.getValue());
}