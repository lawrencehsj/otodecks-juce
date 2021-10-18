/*
  ==============================================================================

    CSVManager.h
    Created: 28 Feb 2021 6:17:58pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once
#include "TrackEntry.h"
#include <vector>
#include <string>

/* this class handles the writing and reading from the file for the saving/saved library */
class CSVManager
{
    public:
    
        CSVManager();
        
        /* reading from file to retrieve TrackEntry objects based on CSV strings in file */
        static std::vector<TrackEntry> readCSV(File* resourceFile);
        /* for save function: write <TrackEntry> data to existing file, if not creates new file to store <TrackEntry> data as CSV strings */
        static void writeCSV(File* resourceFile, std::vector<TrackEntry>* trackEntries);
        /* reads line in file one by one and retrieve available tokens for TrackEntry objects */
        static std::vector<juce::String> tokenise(juce::String csvLine, char separator);
    
    private:
    
        /* reading line from csv file -> TrackEntry object */
        static TrackEntry stringsToTrackEntry(std::vector<juce::String> strings);
     
};
