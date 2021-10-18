/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() 
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 500);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    deckGUI1.deckIndex.setText("DeckGUI 1", dontSendNotification);
    deckGUI2.deckIndex.setText("DeckGUI 2", dontSendNotification);

    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(revFilterGUI);
    //call this to format the audio files, able to read .mp3 etc
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
//    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
//    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
        
    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);
    
    revFilter.prepareToPlay(samplesPerBlockExpected, sampleRate);
 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    revFilter.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources(); //not necessary
    revFilter.releaseResources();
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0,0,getWidth()*0.65,getHeight()*0.5);
    deckGUI2.setBounds(0,getHeight()*0.5,getWidth()*0.65,getHeight()*0.5);
    playlistComponent.setBounds(getWidth()*0.65,0,getWidth()*0.35,getHeight()*0.55);
    revFilterGUI.setBounds(getWidth()*0.65, getHeight()*0.55, getWidth()*0.35, getHeight()*0.5);
}
