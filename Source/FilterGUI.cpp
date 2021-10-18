/*
  ==============================================================================

    FilterGUI.cpp
    Created: 7 Mar 2021 2:42:45am
    Author:  Lawrence Ho

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FilterGUI.h"
#include "Init.h"

/*=====================================================================================*/
IIRFilterGUI::IIRFilterGUI(IIR_Filter *_filter) : filter(_filter)
{
    /* SLIDERS */
    Init::sliderOptions(this, &freqSlider, this, Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow, false, 100, 15, 1, 5000, 0.01, &v1, freqSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &qSlider, this, Slider::RotaryHorizontalDrag, juce::Slider::TextBoxBelow, false, 100, 15, 1, 10, 0.01, &v1, qSlider.textBoxOutlineColourId, Colours::transparentWhite);
    
    //value
    freqSlider.setValue(2500);
    qSlider.setValue(5);
    

    /* COMBO BOX */
    addAndMakeVisible(&filterMenu);
    filterMenu.addItem("Low-Pass", 1);
    filterMenu.addItem("Band-Pass", 2);
    filterMenu.addItem("High-Pass", 3);
    //id
    filterMenu.setSelectedId(1);
    //listener
    filterMenu.addListener(this);
   
    
    /* LABEL */
    Init::labelOptions(this, &label, "IIR Filter", dontSendNotification, Justification::centredLeft, 14.0f, label.textColourId, Colours::lightgrey);
}

IIRFilterGUI::~IIRFilterGUI()
{
    //remove lookandfeel object from sliders to prevent component from using a non existent lookandfeel object upon destruction
    freqSlider.setLookAndFeel(nullptr);
    qSlider.setLookAndFeel(nullptr);
}

void IIRFilterGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withSaturationHSL(0.15).darker());

    g.setColour (juce::Colours::darkgrey);
//    g.drawRect(getLocalBounds(),1);   // draw an outline around the component
    g.setColour (Colours::grey);
    g.drawRect(label.getBounds());
    
    g.setColour (juce::Colours::lightgrey);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void IIRFilterGUI::resized()
{
//    double w = getWidth()/6;

    /* ORIGINAL */
    filterMenu.setBounds(getWidth()*0.2, getHeight()*0.185, getWidth()*0.6, getHeight()*0.1);
    freqSlider.setBounds(getWidth()*0.05, getHeight()*0.2, getWidth()*0.4, getHeight()*0.75);
    qSlider.setBounds(getWidth()*0.5, getHeight()*0.2, getWidth()*0.4, getHeight()*0.75);
    
    /* LABEL */
    label.setBounds(0, 0, getWidth(), getHeight()*0.115);
}

/* get freq/q value from respective sliders */
void IIRFilterGUI::sliderValueChanged (Slider* slider)
{
    if(slider == &freqSlider)
    {
        //set the freq variable for IIR coefficients based on freqSlider value
        filter->freq = slider->getValue();
        //check for comboBox selection
        comboBoxChanged(&filterMenu);
    }
    if(slider == &qSlider)
    {
        //set the q variable for IIR coefficients based on qSlider value
        filter->q = slider->getValue();
        //check for comboBox selection
        comboBoxChanged(&filterMenu);
    }
}

/* sets the correct filter based on the user's selection */
void IIRFilterGUI::comboBoxChanged(ComboBox *comboBox)
{
    if(comboBox == &filterMenu)
    {
        int state = comboBox->getSelectedId();
        switch(state)
        {
            //setLowPass IIRCoefficients
            case lowpass:
            {
                filter->setLowPass();
                break;
            }
            //setBandPass IIRCoefficients
            case bandpass:
            {
                filter->setBandPass();
                break;
            }
            //setHighPass IIRCoefficients
            case highpass:
            {
                filter->setHighPass();
                break;
            }
        }
    }
}




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NEXT CLASS BELOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




/*=====================================================================================*/
RevFilterGUI::RevFilterGUI(RevFilter* _revFilter) : revfilter(_revFilter)
{
    /* SLIDERS */
    Init::sliderOptions(this, &roomSizeSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &dampingSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &wetLevelSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &dryLevelSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &widthSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);
    Init::sliderOptions(this, &freezeModeSlider, this, Slider::LinearVertical, Slider::TextBoxBelow, false, 50, 10, 0.0, 1.0, 0.01, &v1, roomSizeSlider.textBoxOutlineColourId, Colours::transparentWhite);

    /* LABEL */
    Init::labelOptions(this, &label, "Reverb Audio Filter", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &roomSize, "R", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &damping, "DMP", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &wetLevel, "WL", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &dryLevel, "DL", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &width, "W", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);
    Init::labelOptions(this, &freezeMode, "FM", dontSendNotification, Justification::centred, 14.0f, label.textColourId, Colours::lightgrey);

    //labels attach to slider
    roomSize.attachToComponent(&roomSizeSlider, false);
    damping.attachToComponent(&dampingSlider, false);
    wetLevel.attachToComponent(&wetLevelSlider, false);
    dryLevel.attachToComponent(&dryLevelSlider, false);
    width.attachToComponent(&widthSlider, false);
    freezeMode.attachToComponent(&freezeModeSlider, false);
        
    /* IMAGEBUTTON */
    auto infoImage = ImageCache::getFromMemory(BinaryData::info_png, BinaryData::info_pngSize);
    infoButton.setImages(true, true, true, infoImage, 1, Colours::lightgrey, Image(nullptr), 1, Colours::transparentWhite, Image(nullptr), 1, Colours::transparentBlack);
    addAndMakeVisible(infoButton);
    infoButton.setTooltip("R: Room-size, DMP: Damping, WL: Wet-level, DL: Dry-level, W: Width, FM: Freeze-mode");
}

RevFilterGUI::~RevFilterGUI()
{
    //remove lookandfeel object from sliders to prevent component from using a non existent lookandfeel object upon destruction
    roomSizeSlider.setLookAndFeel(nullptr);
    dampingSlider.setLookAndFeel(nullptr);
    wetLevelSlider.setLookAndFeel(nullptr);
    dryLevelSlider.setLookAndFeel(nullptr);
    widthSlider.setLookAndFeel(nullptr);
    freezeModeSlider.setLookAndFeel(nullptr);
}

void RevFilterGUI::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId).withMultipliedBrightness(0.6));
//    g.fillAll(Colours::darkgrey.darker());
    g.setColour (juce::Colours::darkgrey);
    g.drawRect(getLocalBounds(),1);   // draw an outline around the component
    g.setColour (Colours::grey);
    g.drawRect(label.getBounds());

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void RevFilterGUI::resized()
{
    double w = getWidth()/6;
    double h = getHeight() * 0.2;
    
    /* SLIDER */
    roomSizeSlider.setBounds(0, h, w, h*3.25);
    dampingSlider.setBounds(w, h, w, h*3.25);
    wetLevelSlider.setBounds(w*2, h, w, h*3.25);
    dryLevelSlider.setBounds(w*3, h, w, h*3.25);
    widthSlider.setBounds(w*4, h, w, h*3.25);
    freezeModeSlider.setBounds(w*5, h, w, h*3.25);
    
    /* LABEL */
    label.setBounds(0, 0, getWidth(), getHeight()*0.1);
    
    /* IMAGE BUTTON */
    infoButton.setBounds(w*5.5, getHeight()*0.02, w*0.5, getHeight()*0.06);
}

/* get params value from respective sliders */
void RevFilterGUI::sliderValueChanged (Slider* slider)
{
    if(slider == &roomSizeSlider)
        revfilter->params.roomSize = slider->getValue();
    
    if(slider == &dampingSlider)
        revfilter->params.damping = slider->getValue();
    
    if(slider == &wetLevelSlider)
        revfilter->params.wetLevel = slider->getValue();
    
    if(slider == &dryLevelSlider)
        revfilter->params.dryLevel = slider->getValue();
    
    if(slider == &widthSlider)
        revfilter->params.width = slider->getValue();
    
    if(slider == &freezeModeSlider)
        revfilter->params.freezeMode = slider->getValue();
}
