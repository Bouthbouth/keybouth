/*
  ==============================================================================

    SineWaveVoice.cpp
    Created: 5 May 2020 6:38:29pm
    Author:  alexb

  ==============================================================================
*/

#include "SineWaveVoice.h"


/***********************************************************************************/
/******************************* PUBLIC METHODS ************************************/
/***********************************************************************************/

/********************************/
/* SineWaveVoice::SineWaveVoice */
/********************************/
SineWaveVoice::SineWaveVoice()
{
}

/***********************************************************************************/

/*********************************/
/* SineWaveVoice::~SineWaveVoice */
/*********************************/

SineWaveVoice::~SineWaveVoice()
{
}

/*******************************/
/* SineWaveVoice::canPlaySound */
/*******************************/
bool SineWaveVoice::canPlaySound(SynthesiserSound* sound)
{
    return dynamic_cast<SineWaveSound*> (sound) != nullptr;
}


/***********************************************************************************/

/****************************/
/* SineWaveVoice::startNote */
/****************************/

void SineWaveVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/)
{
    /* Set Initial values */
    currentAngle            = 0.0;
    level                   = velocity * 0.15;
    tailOff                 = 0.0;

    /* Convert Midi note number into frequency info */
    auto cyclesPerSecond    = MidiMessage::getMidiNoteInHertz(midiNoteNumber);
    auto cyclesPerSample    = cyclesPerSecond / getSampleRate();

    /* Updates the angle delta */
    angleDelta              = cyclesPerSample * MathConstants<double>::twoPi;
}

/***********************************************************************************/

/***************************/
/* SineWaveVoice::stopNote */
/***************************/

void SineWaveVoice::stopNote(float /*velocity*/, bool allowTailOff)
{
    if (allowTailOff)
    {
        /* tailOff is allowed --> sound will fade out, 
           calling clearCurrentNote() when it's finished. */

        if (tailOff == 0.0)
        {
            /* we only need to begin a tail-off if it's not already doing so - the
               stopNote method could be called more than once. */
            tailOff = 1.0;
        }
            
    }
    else
    {
        /* No tail off --> sound is stopped immediatly and reset everything */
        clearCurrentNote();
        angleDelta = 0.0;
    }
}

/***********************************************************************************/

/**********************************/
/* SineWaveVoice::pitchWheelMoved */
/**********************************/

void SineWaveVoice::pitchWheelMoved(int /*newValue*/)
{
    // not implemented for the purposes of this demo!
}

/***********************************************************************************/

/**********************************/
/* SineWaveVoice::controllerMoved */
/**********************************/

void SineWaveVoice::controllerMoved(int /*controllerNumber*/, int /*newValue*/)
{
    // not implemented for the purposes of this demo!
}

/***********************************************************************************/

/**********************************/
/* SineWaveVoice::renderNextBlock */
/**********************************/

void SineWaveVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    /* Declarations */
    int i;

    float currentSample = 0.0;

    if (angleDelta != 0.0)
    {
        if (tailOff > 0.0)
        {
            while (--numSamples >= 0)
            {
                currentSample   = (float)(sin(currentAngle) * level * tailOff);

                for (i = outputBuffer.getNumChannels() - 1; i >= 0; --i)
                {
                    outputBuffer.addSample(i, startSample, currentSample);
                }

                currentAngle    = currentAngle + angleDelta;
                startSample     = startSample + 1;

                tailOff         = 0.99*tailOff;

                if (tailOff <= 0.005)
                {
                    /* tells the synth that this voice has stopped */
                    clearCurrentNote();

                    angleDelta  = 0.0;
                    break;
                }
            }
        }
        else
        {
            while (--numSamples >= 0)
            {
                currentSample   = (float)(sin(currentAngle) * level);

                for (i = outputBuffer.getNumChannels() - 1; i >= 0; --i)
                {
                    outputBuffer.addSample(i, startSample, currentSample);
                }
                   

                currentAngle    = currentAngle + angleDelta;
                startSample     = startSample + 1;
            }
        }
    }
}


/************************************************************************************/
/******************************* PRIVATE METHODS ************************************/
/************************************************************************************/

/* ... */