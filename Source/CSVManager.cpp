/*
  ==============================================================================

    CSVManager.cpp
    Created: 28 Feb 2021 6:17:58pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "CSVManager.h"
#include <iostream>
#include <fstream>


CSVManager::CSVManager()
{
}

/* reading from file to retrieve TrackEntry objects based on CSV strings in file */
std::vector<TrackEntry> CSVManager::readCSV(File* resourceFile)
{
    //local <TrackEntry>, to be returned later
    std::vector<TrackEntry> entries;

    //check for valid file
    if(!resourceFile->existsAsFile())
        std::cout<<"File does not exist"<<std::endl;
    
    //create local inputstream to read from file
    //dereference resourceFile
    FileInputStream input(*resourceFile);
    
    //handle input opening error
    if(!input.openedOk())
        std::cout<<"Error getting input"<<std::endl;
    else
    {
        //if there is still available input
        while(!input.isExhausted())
        {
            try
            {
                juce::String line = input.readNextLine(); //'%20' as space here
                //get TrackEntry object
                //removeEscapeChars replaces '%20' with ' '
                TrackEntry entry = stringsToTrackEntry(tokenise(URL::removeEscapeChars(line), ','));
                //push back to local <TrackEntry> entries
                entries.push_back(entry);
            }
            catch(const std::exception& e)
            {
                //if csv in file not according to TrackEntry format
                std::cout << "CSVManager::readCSV bad data"  << std::endl;
            }
        }
        
    }
    std::cout << "CSVManager::readCSV read " << entries.size() << " tracks."  << std::endl;
    return entries;
}

/* for save function: write <TrackEntry> data to existing file, if not creates new file to store <TrackEntry> data as CSV strings */
void CSVManager::writeCSV(File* resourceFile, std::vector<TrackEntry>* trackEntries)
{
    //checker for invalid file
    if (! resourceFile->existsAsFile())
        std::cout<<"File does not exist"<<std::endl;
    
    //temporary file buffer to write to (so can overwrite whatever data is currently in the file)
    //dereference resourceFile
    TemporaryFile tempFile(*resourceFile);
    //create local outputstream to write file
    FileOutputStream output (tempFile.getFile());
    //handle output opening error
    if(!output.openedOk())
        std::cout<<"Error getting output"<<std::endl;

    else
    {
        //write from the start (prevent duplicate writing)
        output.setPosition(0);
        output.truncate();
    }
    for(int i=0; i<trackEntries->size(); i++)
    {
        //parse data from trackEntries[] to juce::String to write to file
        //dereference trackEntries
        juce::String entry = (*trackEntries)[i].toCSV();

        //write to file using outputstream
        output.writeText(entry+"\n",false,false,nullptr);
        //flush because using buffer
        output.flush();
    }
    //write over file with buffer
    bool success = tempFile.overwriteTargetFileWithTemporary();
    if(success)
    {
        //tester
        std::cout<<"File write was successful."<<std::endl;
    }
}

/* reads line in file one by one and retrieve available tokens for TrackEntry objects */
std::vector<juce::String> CSVManager::tokenise(juce::String csvLine, char separator)
{
    //create local <juce::String> to be returned later
    std::vector<juce::String> tokens;
    signed int start, end;
    juce::String token;
    //convert csvLine to std::string for std functions later on
    std::string csvLineStdString = csvLine.toStdString(); //std::string
    
    //go through line by line
    //set start point
    start = int(csvLineStdString.find_first_not_of(separator, 0)); //call std::string functions
    do
    {
        //set end point
        end = int(csvLineStdString.find_first_of(separator, start)); //call std::string functions
        //break if reach the end of the line
        if (start == csvLine.length() || start == end)
            break;
        //else continue to tokenise
        if (end >= 0)
            token = csvLineStdString.substr(start, end - start); //std::string to juce::String
        else
            token = csvLineStdString.substr(start, csvLine.length() - start); //std::string to juce::String
        //get the tokens
        tokens.push_back(token); //juce::String
        start = end + 1;
    }
    while(end > 0);
    
    return tokens;
}

/* reading line from csv file -> TrackEntry var */
TrackEntry CSVManager::stringsToTrackEntry(std::vector<juce::String> tokens)
{
    URL audioURL;
    juce::String duration;

    //if more than 2 tokens
    if (tokens.size() != 2) // bad
        throw std::exception{};
    // we only have 2 tokens: URL and duration
    try
    {
        audioURL = URL{tokens[0]}; //convert token to URL
        duration = tokens[1];
    }
    catch(const std::exception& e)
    {
        std::cout << "CSVManager::stringsToTE Bad URL " << tokens[0]<< std::endl;
        std::cout << "CSVManager::stringsToTE Bad double " << tokens[1]<< std::endl;
        throw;
    }

    //create local TrackEntry object to be returned
    TrackEntry entry{audioURL, duration};
    return entry;
}
