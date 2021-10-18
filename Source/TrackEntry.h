/*
  ==============================================================================

    TrackEntry.h
    Created: 24 Feb 2021 2:23:39pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <string>

/* this class stores the audio file data */
class TrackEntry
{
    public:
        /* constructor takes in URL and juce::string duration */
        TrackEntry(URL _audioURL, juce::String _duration);
    
        /* returns TrackEntry in juce::String for **PRINTING** */
        juce::String toString();
        /* returns TrackEntry in juce::String for **CSV file** */
        juce::String toCSV();
    
    
//    private:
        juce::String title;
        juce::String artist;
        juce::String duration;
        URL audioURL;
};
