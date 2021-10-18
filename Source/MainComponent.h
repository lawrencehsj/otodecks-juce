/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include "TrackEntry.h"
#include "FilterGUI.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
/* this class initialises everything. RevFilter is added here to the &mixerSource */
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    /* audio player logic */
    /* takes in reference to formatManager to create AudioReaderSource */
    DJAudioPlayer player1{&formatManager}; 
    DJAudioPlayer player2{&formatManager};

    /* deckGUI and playlistComponent referencing each other */
    /* takes in reference to player for audio playback */
    /* takes in reference to formatManager and thumbCache for creating WaveFormDisplay object */
    /* takes in reference to playlistComponent to pushback to TrackEntry vector whenever a file is loaded directly into the deck */
    DeckGUI deckGUI1{&player1, &formatManager, &thumbCache, &playlistComponent};
    DeckGUI deckGUI2{&player2, &formatManager, &thumbCache, &playlistComponent};

    /* playlist aka library */
    /* takes in reference to deckGUI for audio playback and controls */
    PlaylistComponent playlistComponent{&deckGUI1, &deckGUI2};

    /* mixer source for two audioSources from two deckGUIs */
    MixerAudioSource mixerSource;
    
    /* Tooltips for ImageButtons on mouse hover */
    TooltipWindow tip{this, 700};
    
    /* REV filter GUI */
    RevFilterGUI revFilterGUI{&revFilter};
    /* reverb audio filter */
    /* takes in reference to mixerSource to wrap filter around it */
    RevFilter revFilter{&mixerSource};
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
