/*
  ==============================================================================

    FilterGUI.h
    Created: 7 Mar 2021 2:42:45am
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Filter.h"
#include "OtherLookAndFeel.h"

/* this class handles GUI interface for the filters */
/* ============================================================================== */
//insert sliders to set freq and q values -> alter mixer source
//pass in mixer source to IIRFilterAudioSource
class IIRFilterGUI  : public juce::Component,
                   public Slider::Listener,
                   public ComboBox::Listener

{
public:
    IIRFilterGUI(IIR_Filter *_filter);
    ~IIRFilterGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* implement Slider::Listener */
    /* get freq/q value from respective sliders */
    void sliderValueChanged (Slider* slider) override;
    /* implement ComboBox::Listener */
    /* sets the correct filter based on the user's selection */
    void comboBoxChanged(ComboBox *comboBoxThatHasChanged) override;
    

private:
    /* two slider variables for freq and q */
    Slider freqSlider;
    Slider qSlider;
    /* IIR filter */
    IIR_Filter* filter;
    /* customise slider appearance */
    OtherLookAndFeelV1 v1;
    /* selection box to choose btwn lowpass, bandpass, highpass */
    ComboBox filterMenu;
    /* enums for comboBox state */
    enum FilterTypes
    {
        lowpass = 1, bandpass, highpass
    };
    /* label for IIRFilterGUI */
    Label label;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRFilterGUI)
};




/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ NEXT CLASS BELOW ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/




/*=====================================================================================*/
class RevFilterGUI  : public juce::Component,
                      public Slider::Listener

{
public:
    RevFilterGUI(RevFilter* _revFilter);
    ~RevFilterGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /* implement Slider::Listener */
    /* get params value from respective sliders */
    void sliderValueChanged (Slider* slider) override;

private:
    /* 6 slider values for 6 reverb parameters */
    Slider roomSizeSlider;
    Slider dampingSlider;
    Slider wetLevelSlider;
    Slider dryLevelSlider;
    Slider widthSlider;
    Slider freezeModeSlider;
    /* labels for all the params */
    Label roomSize, damping, wetLevel, dryLevel, width, freezeMode;
    /* customise slider appearance */
    OtherLookAndFeelV1 v1;
    /* label for revFilterGUI */
    Label label;
    /* info icon */
    ImageButton infoButton;
    /* reverb audio filter */
    RevFilter* revfilter;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RevFilterGUI)
};
