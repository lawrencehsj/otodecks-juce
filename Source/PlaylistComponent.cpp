/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 23 Feb 2021 7:12:32pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"
#include "Init.h"
#include <random>

//==============================================================================
PlaylistComponent::PlaylistComponent( DeckGUI* _deckGUI1,
                                      DeckGUI* _deckGUI2
                                    )
: trackFound(false),
  deckGUI1(_deckGUI1),
  deckGUI2(_deckGUI2)
{
    /* create images for buttons */
    auto saveImage = ImageCache::getFromMemory(BinaryData::save_png, BinaryData::save_pngSize);
    auto loadLibraryImage = ImageCache::getFromMemory(BinaryData::folder_png, BinaryData::folder_pngSize);
    auto insertImage = ImageCache::getFromMemory(BinaryData::insert_png, BinaryData::insert_pngSize);
    
    /* set images to buttons */
    saveButton.setImages(true, true, true, saveImage, 1, Colours::lightgrey, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    loadLibraryButton.setImages(true, true, true, loadLibraryImage, 1, Colours::lightgrey, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    insertButton.setImages(true, true, true, insertImage, 1, Colours::lightgrey, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    
    /* BUTTONS */
    Init::buttonOptions(this, &saveButton, this, false, "Save library", 0.75f);
    Init::buttonOptions(this, &loadLibraryButton, this, false, "Load library", 0.75f);
    Init::buttonOptions(this, &insertButton, this, false, "Insert track to library", 0.75f);
    
    /* LABEL */
    Init::labelOptions(this, &label, "Playlist", dontSendNotification, Justification::horizontallyCentred, 14.0f, label.textColourId, Colours::lightgrey);
    label.toBack();
    
    /* TABLELISTBOX */
    tableComponent.getHeader().addColumn("Track title", 1, 150);
    tableComponent.getHeader().addColumn("Duration", 2, 58);
    tableComponent.getHeader().addColumn("Track 1", 3, 48);
    tableComponent.getHeader().addColumn("Track 2", 4, 48);
    tableComponent.getHeader().addColumn("Delete", 5, 48);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);
    
    /* SEARCH BAR */
    addAndMakeVisible(search);
    search.addListener(this);
    search.setTextToShowWhenEmpty("Search", juce::Colours::white);
    search.setJustification(juce::Justification::centredLeft);
}

PlaylistComponent::~PlaylistComponent()
{
}

//component override
void PlaylistComponent::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withSaturationHSL(0.15).darker());

    g.setColour (juce::Colours::darkgrey);
    g.drawRect(getLocalBounds(),1);  // draw an outline around the component
    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

//component override
void PlaylistComponent::resized()
{
    double rowH = getHeight()/8;
    double colW = getWidth()/14;
    tableComponent.setBounds(0, rowH*0.8, getWidth(), getHeight());
    label.setBounds(0, rowH*0.2, getWidth(), rowH*0.4);
    search.setBounds(getWidth()*0.7, rowH*0.1, getWidth()*0.3, rowH*0.6);
    saveButton.setBounds(0, rowH*0.2, colW, rowH*0.4);

    loadLibraryButton.setBounds(colW, rowH*0.2, colW, rowH*0.4);
    insertButton.setBounds(colW*2, rowH*0.2, colW, rowH*0.4);
}

//component override
void PlaylistComponent::buttonClicked(Button*  button)
{
    if(button == &saveButton)
    {
        //get file from directory
        File resourceFile = File::getCurrentWorkingDirectory().getSiblingFile("file.txt");
        //write the audio files data from trackEntries to file
        CSVManager::writeCSV(&resourceFile, &trackEntries);
    }
    else if(button == &loadLibraryButton)
    {
        //reset the boolean condition, to display trackEntries content
        trackFound = false;
        File resourceFile = File::getCurrentWorkingDirectory().getSiblingFile("file.txt");
        //read from CSV file to trackEntries
        trackEntries = CSVManager::readCSV(&resourceFile);
        //update table data displayed 
        tableComponent.updateContent();
    }
    else if(button == &insertButton)
    {
        //add to <TrackEntry> upon valid file selection
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            URL audioURL = URL{chooser.getResult()};
            /* adding to <trackEntries> vector and update tableComponent with new push_back */
            addToTrackEntries(audioURL, deckGUI1->player->getTrackDurationWithoutLoadingIntoDeck(audioURL));
        }
    }
    //for buttons inside of tableComponent rows
    else
    {
        //rowIndex to determine what track to retreive from trackEntries
        int rowIndex = std::stoi(button->getComponentID().toStdString().substr(0,1));
        //colIndex to determine what deck to load track
        int colIndex = std::stoi(button->getComponentID().toStdString().substr(1,2));
        
        //local var to store track to play depending on tableComponent's rowIndex
        URL trackToPlay;
        //assign value to trackCounter for deckGUI to follow (when clicking on ffwdButton)
        trackCounter = rowIndex;

        //if user search successful, tableComponent will display different details
        if(trackFound)
        {
            //get URL from trackEntriesFound
            trackToPlay = trackEntriesFound[rowIndex].audioURL;
        }
        else
        {
            //get URL from trackEntries
            trackToPlay = trackEntries[rowIndex].audioURL;
        }
        
        //load file into different deck depending on which col button is clicked
        if(colIndex == 3)   //deckGUI1
        {
            //reset playButton
            deckGUI1->playButton.setToggleState(false, dontSendNotification);
            deckGUI1->player->loadURL(trackToPlay);
            deckGUI1->waveformDisplay.loadURL(trackToPlay);
            deckGUI1->waveformDisplay.setPositionRelative(deckGUI1->player->getPositionRelative());
            //changing track name displayed in deckGUI
            deckGUI1->trackName.setText(trackToPlay.getFileName(), dontSendNotification);
        }
        if(colIndex == 4)   //deckGUI2
        {
            //reset playButton
            deckGUI2->playButton.setToggleState(false, dontSendNotification);
            deckGUI2->player->loadURL(trackToPlay);
            deckGUI2->waveformDisplay.loadURL(trackToPlay);
            deckGUI2->waveformDisplay.setPositionRelative(deckGUI1->player->getPositionRelative());
            //changing track name displayed in deckGUI
            deckGUI2->trackName.setText(trackToPlay.getFileName(), dontSendNotification);
        }
        //delete from trackEntries vector AND update tableComponent
        if(colIndex == 5)
        {
            trackEntries.erase(trackEntries.begin()+rowIndex);
            tableComponent.updateContent();
//            getTrackEntries(); //tester
        }
    }
}

//TableListBoxModel override
int PlaylistComponent::getNumRows()
{
    //condition for which <TrackEntry> to display depending on whether user has made a search
    if(trackFound)
        return int(trackEntriesFound.size());
    else
        return int(trackEntries.size());
}

//TableListBoxModel override
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected)
{
    if(rowIsSelected)
        g.fillAll(juce::Colours::lightblue);
    else
        g.fillAll(juce::Colours::beige);
}

//TableListBoxModel override
void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
    //condition for which <TrackEntry> to paint depending on whether user has made a search
    if(trackFound)
    {   
        if(columnId == 1) //title
            g.drawText(trackEntriesFound[rowNumber].title, 2, 0, width - 4, height, Justification::centredLeft, true);
        if(columnId == 2) //duration
            g.drawText(trackEntriesFound[rowNumber].duration, 2, 0, width - 4, height, Justification::horizontallyCentred, true);
    }
    else
    {
        if(columnId == 1) //title
            g.drawText(trackEntries[rowNumber].title, 2, 0, width - 4, height, Justification::centredLeft, true);
        if(columnId == 2) //duration
            g.drawText(trackEntries[rowNumber].duration, 2, 0, width - 4, height, Justification::horizontallyCentred, true);
    }
}

//override
Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate)
{
    //colId 3 = load deckGUI1
    //colId 4 = load deckGUI2
    if(columnId == 3 || columnId == 4)
    {
        if(existingComponentToUpdate == nullptr)
        {
            //create textbutton pointer,textbutton is a child class of component, so able to store a text button onto a component pointer
            TextButton* btn = new TextButton{"Load"};
            //unique id -> rowNumber + columnId
            String id{std::to_string(rowNumber)+std::to_string(columnId)};
            btn->setComponentID(id);
            //add itself as a button listener
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    //colId 5 = delete from vector
    if(columnId == 5)
    {
        if(existingComponentToUpdate == nullptr)
        {
            //create deletebutton pointer,deletebutton is a child class of component, so able to store a text button onto a component pointer
            TextButton* btn = new TextButton{"Delete"};
            //unique id -> rowNumber + columnId
            String id{std::to_string(rowNumber)+std::to_string(columnId)};
            btn->setComponentID(id);
            //add itself as a button listener
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }
    return existingComponentToUpdate;
}

//TextEditor override
void PlaylistComponent::textEditorReturnKeyPressed(TextEditor& textEditor)
{
    //get the user's search text
    juce::String searchInput = search.getText();
    searchForTrack(searchInput);
    //updateContent to display updated vector
    tableComponent.updateContent();
    //repaint to show consecutive searches
    repaint();
    //after display trackEntriesFound, remove them from the vector to not show duplicate searches
    trackEntriesFound.clear();
}


void PlaylistComponent::searchForTrack(juce::String searchInput)
{
    //reset trackFound
    trackFound = false;
    for(int i=0; i<trackEntries.size(); i++)
    {
        //check whether any trackEntries contains the string input by the user
        if(trackEntries[i].title.containsIgnoreCase(searchInput))
        {
            //set condition to display search results vector (trackEntriesFound)
            trackFound = true;
            //push back to a different class vector trackEntriesFound
            trackEntriesFound.push_back(trackEntries[i]);
        }
    }
    //if not found, pop up message box to tell user
    if(!trackFound)
        AlertWindow::showMessageBox(AlertWindow::WarningIcon, "Invalid search", "No tracks were found");
}

/* adding to <trackEntries> vector and update tableComponent with new push_back */
void PlaylistComponent::addToTrackEntries(URL audioURL, juce::String duration)
{
    trackEntries.push_back(TrackEntry{audioURL, duration});
    tableComponent.updateContent();
}

/* prints all <TrackEntry> objects */
void PlaylistComponent::printTrackEntries()
{
    for(int i=0; i<trackEntries.size(); i++)
    {
        std::cout<<"Track Entries " << i << ": " << trackEntries[i].toString() <<std::endl;
    }
}

/*  return next track to play when ffwd button is clicked in deckGUI,
    depending on shuffle condition, returns next track in sequence or random track */
URL PlaylistComponent::getNextTrack(bool shuffle)
{
    URL audioURL;
    //check for non-empty trackEntries
    if(trackEntries.size()!=0)
    {
        if(!shuffle)
        {
            //if trackCounter+1 (next track) is lesser than trackEntries.size-1
            //trackEntries -1 because checking for trackCounter++
            if(trackCounter < trackEntries.size()-1)
            {
                //increment counter to get next track index
                trackCounter++;
            }
            //if reach the last index of trackEntries, go back to the start
            else
            {
                trackCounter = 0;
            }
            audioURL = trackEntries[trackCounter].audioURL;
        }
        else
        {
            /* random num generator tools */
            std::random_device rand_dev;
            std::mt19937 generator(rand_dev());
            std::uniform_int_distribution<int>  dist(0, int(trackEntries.size()-1));

            //generate random num
            int r = dist(generator);
            //if r generates the same index as the current playing track, generate another random num
            while(r == trackCounter)
            {
                r = dist(generator);
            }
            
            //load a random URL from trackEntries
            audioURL = trackEntries[r].audioURL;
            //assign trackCounter to r to keep track of current playing song
            trackCounter = r;
        }
    }
    return audioURL;
}

std::vector<TrackEntry>* PlaylistComponent::getTrackEntries()
{
    return &trackEntries;
}
