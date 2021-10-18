/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "Filter.h"

/* this class contains the logic of playing the audio. IIR_Filter is added here to the &resampleSource */
class DJAudioPlayer : public AudioSource {
  public:

    DJAudioPlayer(AudioFormatManager* _formatManager);
    ~DJAudioPlayer();   //D structure, what is this?

    /* AudioSource override */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    /* loadURL from FileChooser and assign AudioFormatSourceReader* to transportSource and readerSource* */
    void loadURL(URL audioURL);
    /* set volume level */
    void setGain(double gain);
    /* set playback speed */
    void setSpeed(double ratio);
    /* set position of transportSource */
    void setPosition(double posInSecs);
    /* set relative position of wfd and posSlider (graphics) */
    void setPositionRelative(double pos);
    /* set audio loop */
    void setLoop(bool b);

    /* audio player functions */
    void start();
    void stop();

    /* get the relative position of the playhead */
    double getPositionRelative();
    /* returns track duration in minutes; but this method only works after transportSource is initialized */
    juce::String getTrackDuration();
    /* returns track duration in minutes without initializing transportSource */
    juce::String getTrackDurationWithoutLoadingIntoDeck(URL audioURL);
    
    /* IIR filter on resample source */
    //public for deckGUI to access
    IIR_Filter filter{&resampleSource};
    
private:
    AudioFormatManager* formatManager;
//    this is a pointer as we dont want to contruct it until we have a file to play
//    it is a unique ptr as that should be what pointers always start with, until they need to be passed around, then change
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;   //this is a type of audioSource
    //the constructor for ResamplingAudioSource receives a pointer to an AudioSource, which is transportSource in this case
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
};




