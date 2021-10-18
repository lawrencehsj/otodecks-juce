/*
  ==============================================================================

    Filter.cpp
    Created: 7 Mar 2021 2:45:20am
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "Filter.h"

/*=====================================================================================*/
IIR_Filter::IIR_Filter(ResamplingAudioSource *resampleSource) : filterSource(resampleSource, false)
{
}

IIR_Filter::~IIR_Filter()
{
    
}

void IIR_Filter::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    globalSampleRate = sampleRate;
    filterSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void IIR_Filter::releaseResources()
{
    filterSource.releaseResources();
}

void IIR_Filter::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    filterSource.getNextAudioBlock(bufferToFill);
}

/* sets coefficients of lowpass IIR filter to be applied to audio source */
void IIR_Filter::setLowPass()
{
    IIRCoefficients lowPassFilter = IIRCoefficients::makeLowPass(globalSampleRate, freq, q);
    filterSource.setCoefficients(lowPassFilter);
}

/* sets coefficients of bandpass IIR filter to be applied to audio source */
void IIR_Filter::setBandPass()
{
    IIRCoefficients bandPassFilter = IIRCoefficients::makeBandPass(globalSampleRate, freq, q);
    filterSource.setCoefficients(bandPassFilter);
}

/* sets coefficients of highpass IIR filter to be applied to audio source */
void IIR_Filter::setHighPass()
{
    IIRCoefficients highPassFilter = IIRCoefficients::makeHighPass(globalSampleRate, freq, q);
    filterSource.setCoefficients(highPassFilter);
}




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NEXT CLASS BELOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




/*=====================================================================================*/
RevFilter::RevFilter(MixerAudioSource* mixerSource) : reverbSource(mixerSource, false)

{
}

RevFilter::~RevFilter()
{
    
}

void RevFilter::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    reverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void RevFilter::releaseResources()
{
    reverbSource.releaseResources();
}

void RevFilter::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
{
    //set params during this callback
    reverbSource.setParameters(params);
    reverbSource.getNextAudioBlock(bufferToFill);
}
