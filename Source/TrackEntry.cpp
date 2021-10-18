/*
  ==============================================================================

    TrackEntry.cpp
    Created: 24 Feb 2021 2:23:39pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "TrackEntry.h"
#include <string>
#include <cmath>

TrackEntry::TrackEntry( URL _audioURL, juce::String _duration)
: audioURL(_audioURL)
{
    //get file name from URL and replace '%20' with ' '
    title = URL::removeEscapeChars(audioURL.getFileName()) ;
    //convert duration to 2sf and replace '.' with ':'
    //e.g 3.149865 -> 3:14
    duration = juce::String(_duration).substring(0, 4).replaceCharacter('.', ':');
}

/* returns TrackEntry in juce::String for PRINTING */
juce::String TrackEntry::toString()
{
    juce::String result = "Title: " + title + ", Duration: " + duration;
    return result;
}

/* returns TrackEntry in juce::String for CSV file */
juce::String TrackEntry::toCSV()
{
    //audioURL and duration only
    return audioURL.toString(true) + "," + duration;
}
