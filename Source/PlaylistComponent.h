/*
  ==============================================================================

    PlaylistComponent.h
    Created: 23 Feb 2021 7:12:32pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "TrackEntry.h"
#include "CSVManager.h"
#include "DeckGUI.h"
#include <vector>
#include <string>

//==============================================================================
/* this class contains logic and appearance of the library (aka playlist) component */
class PlaylistComponent  : public Component,
                           //TableListBoxModel allows the table to get access to data and draw it
                           public TableListBoxModel,
                           public Button::Listener,
                           public TextEditor::Listener,
                           public LookAndFeel_V2
{
public:
    /* takes references of deckGUI and trackEntries from MainComponent */
    PlaylistComponent( DeckGUI* _deckGUI1,
                       DeckGUI* _deckGUI2
                     );
    ~PlaylistComponent() override;

    /* Component override methods */
    void paint (Graphics&) override;
    void resized() override;
    void buttonClicked(Button* button) override;
    
    /* TableListBoxModel override methods */
    int getNumRows() override;
    void paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;
        
    /* TextEditor override function: if search bar input is entered by user*/
    void textEditorReturnKeyPressed(TextEditor& textEditor) override;
        
    /* search for track */
    void searchForTrack(juce::String searchInput);

    /* adding to <trackEntries> vector and update tableComponent with new push_back */
    void addToTrackEntries(URL audioURL, juce::String duration);
    /* prints all <TrackEntry> objects */
    void printTrackEntries();
    /* returns trackEntries */
    std::vector<TrackEntry>* getTrackEntries();
    
    /*  return next track to play when ffwd button is clicked in deckGUI,
        depending on shuffle condition, returns next track in sequence or random track */
    URL getNextTrack(bool shuffle);

    /* public vars for deckGUI to access */
    TableListBox tableComponent;
    /* condition for displaying search results */
    bool trackFound = false;
    /* track counter */
    int trackCounter;

private:
    
    /* <TrackEntry> vector to store loaded songs and save songs */
    std::vector<TrackEntry> trackEntries;
    /* local <TrackEntry> for storing search results */
    std::vector<TrackEntry> trackEntriesFound;
    
    /* pointers to deckGUI created in MainComponent */
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;
    
    /* search bar */
    TextEditor search{"search", 0};
    
    /* buttons */
    ImageButton saveButton;
    ImageButton loadLibraryButton;
    ImageButton insertButton;
    
    /* playlist label */
    Label label;

        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
