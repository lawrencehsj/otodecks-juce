/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/* this class contains the logic and display for wfd */
class WaveformDisplay    : public Component, 
                           public ChangeListener
{
public:
    WaveformDisplay( AudioFormatManager* formatManagerToUse,
                     AudioThumbnailCache* cacheToUse );
    ~WaveformDisplay();

    void paint (Graphics&) override;
    void resized() override;
    /* this method listens for any changes, if any, repaints the updated pos in wfd */
    void changeListenerCallback (ChangeBroadcaster *source) override;
    /* if valid file has been loaded, sets class bool fileLoaded to true (repaint waveform)*/
    void loadURL(URL audioURL);
    /* set the relative position of the playhead*/
    void setPositionRelative(double pos);
    
private:
    AudioThumbnail audioThumb;
    /* condition for repainting */
    bool fileLoaded; 
    double position; //for playhead
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
