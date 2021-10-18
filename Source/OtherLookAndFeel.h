/*
  ==============================================================================

    OtherLookAndFeel.h
    Created: 11 Mar 2021 11:48:23pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

/* referenced from: https://github.com/juce-framework/JUCE/blob/master/examples/GUI/LookAndFeelDemo.h */
/*  https://github.com/remberg/juceCustomSliderSample/blob/master/Source/myLookAndFeel.cpp */
/* this class handles the appearance settings for sliders/buttons in the audio app */
class OtherLookAndFeelV1 : public LookAndFeel_V4
{
    public:
    
        OtherLookAndFeelV1();
    
        /* customise rotary slider appearance -> used in FilterGUI */
        void drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
            float rotaryStartAngle, float rotaryEndAngle, Slider& slider) override;
    
        /* customise slider thumb appearance for FilterGUI */
        void drawRoundThumb (Graphics& g, float x, float y, float diameter, Colour colour, float outlineThickness);
    
        void drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                    float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle style, Slider& slider) override;
        
        /* customise slider appearance for FilterGUI */
        void drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                                   float sliderPos, float minSliderPos, float maxSliderPos,
                                                   const Slider::SliderStyle style, Slider& slider) override;
    
        /* customise slider background for FilterGUI */
        void drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                         float /*sliderPos*/,
                                         float /*minSliderPos*/,
                                         float /*maxSliderPos*/,
                                     const Slider::SliderStyle /*style*/, Slider& slider) override;
};
