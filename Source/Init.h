/*
  ==============================================================================

    Init.h
    Created: 13 Mar 2021 2:43:02pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "OtherLookAndFeel.h"


/* this class contains helper fns to initialise slider/label/button settings */
/* initialises common settings for repetitive calls to tidy up code */
class Init : public Component,
             public Slider::Listener,
             public Button::Listener,
             public ComboBox::Listener
{
    public:
        Init();
    
        /* helper fn to initialise slider settings */
        static void sliderOptions(Component *component, Slider *slider, Slider::Listener *listener, Slider::SliderStyle style, Slider::TextEntryBoxPosition textPos, bool readOnly, int textBoxW, int textBoxH, double rangeStart, double rangeEnd, double increment, LookAndFeel *otherLookAndFeel, Slider::ColourIds colourid, Colour colour);
    
        /* helper fn to initialise label settings */
        static void labelOptions(Component *component, Label *label, String title, NotificationType notifType, Justification justification, float fontSize, Label::ColourIds colourid, Colour colour);
        
        /* helper fn to initialise button settings */
        static void buttonOptions(Component *component, Button *button, Button::Listener *listener, bool toggleOn, String tooltip, float alpha);
      
};
