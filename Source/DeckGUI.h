/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "TrackEntry.h"
#include "FilterGUI.h"

//==============================================================================
/* this class contains the layout for the music player */
class PlaylistComponent;

class DeckGUI    : public Component,
                   public Button::Listener,
                   public Slider::Listener,
                   public FileDragAndDropTarget, 
                   public Timer
{
public:
    DeckGUI( DJAudioPlayer *_player,
             AudioFormatManager* formatManagerToUse,
             AudioThumbnailCache* cacheToUse,
             PlaylistComponent *_playlistComponent
            );
    ~DeckGUI();

    /* Component override*/
    void paint (Graphics&) override;
    void resized() override;
    
     /* implement Button::Listener */
    void buttonClicked (Button* button) override;
    /* implement Slider::Listener */
    void sliderValueChanged (Slider* slider) override;
    /* implement file draganddrop to drag and load files*/
    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override;
    /* implement timer */
    void timerCallback() override;
    
    /* public vars for playlistComponent to access */
    WaveformDisplay waveformDisplay;
    DJAudioPlayer* player;
    /* misc labels */
    Label deckIndex;
    Label trackName;
    
    /* public for playlistComp to access */
    ImageButton playButton;

private:

    /* main buttons */
    ImageButton loopButton;
    ImageButton frevButton;
    ImageButton ffwdButton;
    ImageButton shuffleButton; //shuffle
    /* menu buttons */
    ImageButton volumeButton;
    ImageButton loadButton;
        
    Slider volSlider; 
    Slider speedSlider;
    Slider posSlider;
        
    /* pointer to PC created in MainComponent */
    PlaylistComponent* playlistComponent;
    /* reference to trackEntries created in MainComponent */
    std::vector<TrackEntry>* trackEntries;
    
    /* IIR filter and GUI */
    IIRFilterGUI iirFilterGUI{filter};
    IIR_Filter *filter;
    
    /* shuffle condition */
    bool shuffle;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
