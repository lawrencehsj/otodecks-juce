/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay( AudioFormatManager* formatManagerToUse,
                                  AudioThumbnailCache* cacheToUse)
: audioThumb(1000, *formatManagerToUse, *cacheToUse),
  fileLoaded(false),
  position(0)
{
  // add the change listener to wfd
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

/*  paint is only called when the window is resized or when u recall repaint.
    when we called repaint, the waveform had not been plotted yet
    tldr, paint not called continually */
void WaveformDisplay::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withSaturationHSL(0.1));
    g.setColour (Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::beige);
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.setColour(Colours::orange.brighter());
      g.drawLine(position * getWidth(), 0, position * getWidth(), getHeight(),1.5);
    }
    else 
    {
      g.setFont (20.0f);
      g.drawText ("File not loaded...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
}

/* if valid file has been loaded, sets class bool fileLoaded to true (repaint waveform)*/
void WaveformDisplay::loadURL(URL audioURL)
{
    //clear the old one in case its drawn anyth previously
    audioThumb.clear();
    //unpack audioURL, turn it into an InputSource, then call setSource
    fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        repaint();
    }
    else {
        std::cout << "wfd: not loaded! " << std::endl;
    }

}

/* this method listens for any changes, if any, repaints the updated pos in wfd */
void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    repaint();
}

/* set position of playhead */
void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}
