/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager* _formatManager)
: formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
//    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
//    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    filter.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
//    resampleSource.getNextAudioBlock(bufferToFill);
    filter.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
//    to complete the life cycle of transportSource
    transportSource.releaseResources();
    resampleSource.releaseResources();
    filter.releaseResources();
}

/* loadURL from FileChooser and assign AudioFormatSourceReader* to transportSource and readerSource* */
void DJAudioPlayer::loadURL(URL audioURL)
{
    auto* reader = formatManager->createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {       
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        //reset to delete, release doesnt delete
        readerSource.reset (newSource.release());
        std::cout << "File loaded successfully." << std::endl;
    }
    else
    {
      std::cout << "Something went wrong loading the file " << std::endl;
    }
}

/* set volume level */
void DJAudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}

/* set playback speed */
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 10.0)
    {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 10" << std::endl;
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}

/* set position of transportSource */
void DJAudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
}

/* set relative position of wfd and posSlider (graphics) */
void DJAudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
        std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

/* set audio loop */
void DJAudioPlayer::setLoop(bool b)
{
    //if readerSource is not null
    if(readerSource)
        readerSource->setLooping(b);
}

void DJAudioPlayer::start()
{
    transportSource.start();
}
void DJAudioPlayer::stop()
{
    transportSource.stop(); //pausing only
}

/* get the relative position of the playhead */
double DJAudioPlayer::getPositionRelative()
{
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

/* returns track duration in minutes; but this method only works after transportSource is initialized */
juce::String DJAudioPlayer::getTrackDuration()
{
    //first convert length in seconds to min (but decimal portion not converted)
    double tempLengthInMin = transportSource.getLengthInSeconds() / 60;
    //get the decimal portion of the number and convert to minutes format (1 min = 60 sec)
    double secToMin = (tempLengthInMin - floor(tempLengthInMin) ) * 0.6;
    //get the length in min (full conversion)
    double _duration = (floor(tempLengthInMin) + secToMin );
    //convert to string
    juce::String duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
    return duration;

}

/* returns track duration in minutes without initializing transportSource */
juce::String DJAudioPlayer::getTrackDurationWithoutLoadingIntoDeck(URL audioURL)
{
    juce::String duration;
    auto* reader = formatManager->createReaderFor(audioURL.createInputStream(false));
    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader, true));
        //length of track = lengthInSamples / sampleRate
        //first convert length in seconds to min (but decimal portion not converted)
        double tempLengthInMin = (reader->lengthInSamples / reader->sampleRate) /60;
        //get the decimal portion of the number and convert to minutes format (1 min = 60 sec)
        double secToMin = (tempLengthInMin - floor(tempLengthInMin) ) * 0.6;
        //get the length in min (full conversion)
        double _duration = (floor(tempLengthInMin) + secToMin );
        //convert to string
        duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
        //delete newSource to prevent leaking
        //destroy newSource -> destroy reader
        newSource.reset();
    }
    return duration;
}
