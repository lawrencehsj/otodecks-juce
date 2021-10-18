/*
  ==============================================================================

    Init.cpp
    Created: 13 Mar 2021 2:43:02pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "Init.h"

Init::Init()
{
    
}

/* helper fn to initialise slider settings */
void Init::sliderOptions(Component *component, Slider *slider, Slider::Listener *listener, Slider::SliderStyle style, Slider::TextEntryBoxPosition textPos, bool readOnly, int textBoxW, int textBoxH, double rangeStart, double rangeEnd, double increment, LookAndFeel *otherLookAndFeel, Slider::ColourIds colourid, Colour colour)
{
    /*  addAndMakeVisible
        addListener
        setSliderStyle
        setTextBoxStyle
        setColour
        setRange
        setLookAndFeel
        setColour */
    component->addAndMakeVisible(slider);
    slider->addListener(listener);
    slider->setSliderStyle(style);
    slider->setTextBoxStyle(textPos, readOnly, textBoxW, textBoxH);
    slider->setColour(slider->textBoxOutlineColourId, Colours::transparentWhite);
    slider->setRange(rangeStart, rangeEnd, increment);
    slider->setLookAndFeel(otherLookAndFeel);
    slider->setColour(colourid, colour);
}

/* helper fn to initialise label settings */
void Init::labelOptions(Component *component, Label *label, String title, NotificationType notifType, Justification justification, float fontSize, Label::ColourIds colourid, Colour colour)
{
    /*  addAndMakeVisible
        setText
        setJustificationType
        setFont
        setColour */
    component->addAndMakeVisible(label);
    label->setText(title, notifType);
    label->setJustificationType(justification);
    label->setFont(fontSize);
    label->setColour(colourid, colour);
}

/* helper fn to initialise button settings */
void Init::buttonOptions(Component *component, Button *button, Button::Listener *listener, bool toggleOn, String tooltip, float alpha)
{
    /*  addAndMakeVisible
        addListener
        setClickingTogglesState
        setTooltip
        setAlpha */
    component->addAndMakeVisible(button);
    button->addListener(listener);
    button->setClickingTogglesState(toggleOn);
    button->setTooltip(tooltip);
    button->setAlpha(alpha);
}
