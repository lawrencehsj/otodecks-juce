/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"
#include <random>
#include "Init.h"


//==============================================================================
DeckGUI::DeckGUI( DJAudioPlayer* _player,
                  AudioFormatManager* formatManagerToUse,
                  AudioThumbnailCache* cacheToUse,
                  PlaylistComponent* _playlistComponent
                )
: waveformDisplay(formatManagerToUse, cacheToUse),
  player(_player),
  playlistComponent(_playlistComponent),
  trackEntries(playlistComponent->getTrackEntries()),
  iirFilterGUI(&player->filter)
{
    /* create images for buttons */
    auto playImage = ImageCache::getFromMemory(BinaryData::play_png, BinaryData::play_pngSize);
    auto pauseImage = ImageCache::getFromMemory(BinaryData::pause_png, BinaryData::pause_pngSize);
    auto loadImage = ImageCache::getFromMemory(BinaryData::file_png, BinaryData::file_pngSize);
    auto ffwdImage = ImageCache::getFromMemory(BinaryData::fast_forward_png, BinaryData::fast_forward_pngSize);
    auto frevImage = ImageCache::getFromMemory(BinaryData::fast_reverse_png, BinaryData::fast_reverse_pngSize);
    auto loopImage = ImageCache::getFromMemory(BinaryData::loop_png, BinaryData::loop_pngSize);
    auto volumeImage = ImageCache::getFromMemory(BinaryData::volume_png, BinaryData::volume_pngSize);
    auto muteImage = ImageCache::getFromMemory(BinaryData::mute_png, BinaryData::mute_pngSize);
    auto shuffleImage = ImageCache::getFromMemory(BinaryData::shuffle_png, BinaryData::shuffle_pngSize);
    
    /* set images to buttons */
    playButton.setImages(true, true, true, playImage, 1, Colours::blanchedalmond, Image(nullptr), 1, Colours::transparentWhite, pauseImage, 1, Colours::beige);
    loadButton.setImages(true, true, true, loadImage, 1, Colours::lightgrey, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    ffwdButton.setImages(true, true, true, ffwdImage, 0.2, Colours::blanchedalmond, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    frevButton.setImages(true, true, true, frevImage, 1, Colours::blanchedalmond, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    loopButton.setImages(true, true, true, loopImage, 1, Colours::blanchedalmond, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    volumeButton.setImages(true, true, true, volumeImage, 1, Colours::lightyellow, Image(nullptr), 1, Colours::transparentWhite, muteImage, 1, Colours::transparentBlack);
    shuffleButton.setImages(true, true, true, shuffleImage, 1, Colours::blanchedalmond, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);

    /* BUTTONS */
    Init::buttonOptions(this, &playButton, this, true, "", 0.8f);
    Init::buttonOptions(this, &loadButton, this, false, "Insert track to DeckGUI", 0.8f);
    Init::buttonOptions(this, &ffwdButton, this, false, "Plays next track in playlist", 0.8f);
    Init::buttonOptions(this, &frevButton, this, false, "Goes back to start of track", 0.8f);
    Init::buttonOptions(this, &loopButton, this, true, "Loop current track", 0.8f);
    Init::buttonOptions(this, &volumeButton, this, true, "", 0.8f);
    Init::buttonOptions(this, &shuffleButton, this, true, "Enable shuffing", 0.8f);


    /* LABELS */
    Init::labelOptions(this, &deckIndex, "", NotificationType::dontSendNotification, Justification::horizontallyCentred, 14.0f, deckIndex.outlineColourId, getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withMultipliedBrightness(0.6f));
    Init::labelOptions(this, &trackName, "", NotificationType::dontSendNotification, Justification::horizontallyCentred, 14.0f, deckIndex.outlineColourId, getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withMultipliedBrightness(0.6f));
    
    //customise
    trackName.setAlpha(0.8);
    deckIndex.setAlpha(0.8);
    deckIndex.toBack();

    /* SLIDERS */
    Init::sliderOptions(this, &volSlider, this, juce::Slider::LinearHorizontal, juce::Slider::NoTextBox, true, 0, 0, 0.0, 1.0, 0.01, nullptr, volSlider.textBoxTextColourId, Colours::lightgrey);
    Init::sliderOptions(this, &speedSlider, this, juce::Slider::IncDecButtons, juce::Slider::TextBoxAbove, true, 50, 50, 0.0, 10.0, 0.5, nullptr, speedSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &posSlider, this, juce::Slider::LinearHorizontal, juce::Slider::NoTextBox, true, 0, 0, 0.0, 1.0, 0, nullptr, posSlider.trackColourId, Colours::lightblue.withBrightness(0.7));

    /* MISC */
    //value
    volSlider.setValue(1.0);
    speedSlider.setValue(1.0);
    //tooltip
    speedSlider.setTooltip("Adjust playback speed");
    speedSlider.setTextBoxIsEditable(true);
    //text
    speedSlider.setTextValueSuffix("x");
    /* WFD */
    addAndMakeVisible(waveformDisplay);
    /* start the timer thread */
    startTimer(100); //parameter takes in milliseconds; how frequent u want to call this function
    
    /* FilterGUI */
    addAndMakeVisible(iirFilterGUI);
    iirFilterGUI.toBack();
}

DeckGUI::~DeckGUI()
{
    /* stop the timer when we get destroyed */
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withSaturationHSL(0.1).darker());
    g.setColour (Colours::darkgrey);
    g.drawRect(getLocalBounds(),1);  // draw an outline around the component
    
    g.drawRect(trackName.getBounds(),1);
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
    
    //label bg colour
//    g.setColour(getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withMultipliedBrightness(0.6f));
//    g.fillRect(deckIndex.getBounds());
}

void DeckGUI::resized()
{
    //seperate player and filterGUI
    double deckW = getWidth()*0.7;
    
    /* MISC */
    deckIndex.setBounds(0, 0, getWidth(), getHeight()*0.1);
    loadButton.setBounds(0, getHeight()*0.02, deckW*0.05, getHeight()*0.07);

    /* BUTTONS */
    shuffleButton.setBounds(deckW*0.29, getHeight()*0.815, deckW*0.06, getHeight()*0.08);
    frevButton.setBounds(deckW*0.37, getHeight()*0.795, deckW*0.08, getHeight()*0.12);
    playButton.setBounds(deckW*0.45, getHeight()*0.75, deckW*0.1, getHeight()*0.2);
    ffwdButton.setBounds(deckW*0.55, getHeight()*0.795, deckW*0.08, getHeight()*0.12);
    loopButton.setBounds(deckW*0.63, getHeight()*0.81, deckW*0.08, getHeight()*0.08);
    volumeButton.setBounds(deckW*0.04, getHeight()*0.8, deckW*0.025, getHeight()*0.12);

    /* SLIDERS */
    posSlider.setBounds(deckW*0.05, getHeight()*0.9, deckW*0.9, getHeight()*0.08);
    volSlider.setBounds(deckW*0.065, getHeight()*0.82, deckW*0.18, getHeight()*0.08);
    speedSlider.setBounds(deckW*0.75, getHeight()*0.78, deckW*0.2, getHeight()*0.12);
    
    /* SUB-MENU LAYOUT */
    iirFilterGUI.setBounds(deckW, getHeight()*0.1, getWidth()*0.3, getHeight()*0.9);
    waveformDisplay.setBounds(0, getHeight()*0.1, deckW, getHeight()*0.5);
    trackName.setBounds(0, getHeight()*0.6, deckW, getHeight()*0.1);
}

void DeckGUI::buttonClicked(Button* button)
{
    //reset search to display trackEntries (NOT the search results vector)
    playlistComponent->trackFound = false;

    if (button == &playButton) //if button same as memory address of ____
    {
        //starts the audio player if button set to true, pauses if false
        //if playButton toggle state = true (pause img)
        if(button->getToggleState()) 
            player->start();
        else //false
            player->stop();
    }
    //load file into deckGUI and playlist
    if (button == &loadButton)
    {
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            URL audioURL = URL{chooser.getResult()};
            player->loadURL(audioURL);
            waveformDisplay.loadURL(audioURL);
            trackName.setText(audioURL.getFileName(), dontSendNotification);
            //adding to trackEntries vector and updating tableComponent in playlistComponent
            playlistComponent->addToTrackEntries(audioURL, player->getTrackDuration());
            // playlistComponent->printTrackEntries(); //tester code
        }
    }
    //reverse button (go back to start)
    if (button == &frevButton)
    {
        player->stop();
        player->setPosition(0);
        player->start();
        //change state of playButton to display pause img
        playButton.setToggleState(true, dontSendNotification);
    }
    //forward button (go to next track in trackEntries)
    if (button == &ffwdButton)
    {
        //reset loop button
        loopButton.setToggleState(false, dontSendNotification);
        //does nothing if empty, error handler
        if(trackEntries->size()==0)
            return;
        
        //stops the player, gets the URL of the next track, loads it, and then plays it
        player->stop();
        //get the next track based on the trackCounter index in trackEntries
        URL audioURL = playlistComponent->getNextTrack(shuffle);
        player->loadURL(audioURL);
        waveformDisplay.loadURL(audioURL);
        //change the trackName to new loaded track
        trackName.setText(audioURL.getFileName(), dontSendNotification);
        player->start();
        //reset the playButton state
        playButton.setToggleState(true, dontSendNotification);
    }
    //loop track
    if(button == &loopButton)
    {
        //if loopButton toggled on, audio will be looped
        player->setLoop(button->getToggleState());
//        std::cout<<"shuffle state:: "<<shuffleButton.getToggleState()<<std::endl;

    }
    //mute/unmute button
    if(button == &volumeButton)
    {
        //if button is clicked (true) -> change button image to mute image and adjust gain value according to slider value
        if(button->getToggleState())
        {
            player->setGain(0);
            volSlider.setValue(0);
        }
        //else if button state is (false), change button image to unmute image and adjust gain value according to slider value
        else
        {
            player->setGain(1);
            volSlider.setValue(1);
        }
    }
    //sets shuffle condition
    if(button == &shuffleButton)
    {
        if(button->getToggleState())
            shuffle = true;
        else //false
            shuffle = false;
        
//        std::cout<<"shuffle state:: "<<button->getToggleState()<<std::endl;
    }
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volSlider)
    {
        //as long as volume > 0, volumeButton state = false (unmuted)
        if(slider->getValue()>slider->getMinimum())
        {
            volumeButton.setToggleState(false, dontSendNotification);
        }
        //adjusts volume of audio playback depending on slider value
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        //set the speed of playback based on slider value
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &posSlider)
    {
        if(slider->getValue() == slider->getMaximum())
        {
            //if reach end of track, reset playButton
            playButton.setToggleState(false, dontSendNotification);
        }
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
    player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    //check for nan (if no track is being loaded)
    if(!isnan(player->getPositionRelative()))
    {
        //move posSlider position together with waveformDisplay according to position relative to track
        waveformDisplay.setPositionRelative(player->getPositionRelative());
        posSlider.setValue(player->getPositionRelative());
    }
}
