/*
  ==============================================================================

    Filter.h
    Created: 7 Mar 2021 2:45:20am
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

/* this class handles the different filter types present in the audio app */
class IIR_Filter : public AudioSource
{
    public:
        IIR_Filter(ResamplingAudioSource *resampleSource);
        ~IIR_Filter();
    
        /* override */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void releaseResources() override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        /* state of filter depends on comboBox's selected option */
        void filterMenuChanged();
    
        /* sets coefficients of IIR filter to be applied to audio source */
        void setLowPass();
        void setBandPass();
        void setHighPass();
    
        /* variables for filter on start up */
        double freq = 2500, q = 25.0f;
    
    private:
        /* filterSource to apply on &resampleSource (reference to resampleSource in deckGUI */
        IIRFilterAudioSource filterSource;
        /* global var to store sample rate from prepareToPlay to pass into IIR filter */
        double globalSampleRate;
};




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NEXT CLASS BELOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




/*=====================================================================================*/
class RevFilter : public AudioSource
{
    public:
        RevFilter(MixerAudioSource* mixerSource);
        ~RevFilter();
    
        /* override */
        void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
        void releaseResources() override;
        void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
        /* state of filter depends on comboBox's selected option */
        void filterMenuChanged();
        /* parameters for reverb audio filter */
        Reverb::Parameters params;
    
    private:

        /* reverb audio source to be applied to audio source */
        /* apply to &mixerSource in MainComponent */
        ReverbAudioSource reverbSource;
};
