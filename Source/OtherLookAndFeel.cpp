/*
  ==============================================================================

    OtherLookAndFeel.cpp
    Created: 11 Mar 2021 11:48:23pm
    Author:  Lawrence Ho

  ==============================================================================
*/

#include "OtherLookAndFeel.h"

/* referenced from: https://github.com/juce-framework/JUCE/blob/master/examples/GUI/LookAndFeelDemo.h */
/*  https://github.com/remberg/juceCustomSliderSample/blob/master/Source/myLookAndFeel.cpp */
OtherLookAndFeelV1::OtherLookAndFeelV1()
{
    
}

/* customise rotary slider appearance -> used in FilterGUI */
void OtherLookAndFeelV1::drawRotarySlider(Graphics& g, int x, int y, int width, int height, float sliderPos,
                      float rotaryStartAngle, float rotaryEndAngle, Slider& slider)
{
    const float radius = jmin(width / 2, height / 2) *0.85f;
    const float centreX = x + width * 0.5f;
    const float centreY = y + height * 0.5f;
    const float rx = centreX - radius;
    const float ry = centreY - radius;
    const float rw = radius * 2.0f;
    const float angle = rotaryStartAngle
        + sliderPos
        * (rotaryEndAngle - rotaryStartAngle);

    g.setColour(Colour(0xff39587a));
    Path filledArc;
    filledArc.addPieSegment(rx, ry, rw + 1, rw + 1, rotaryStartAngle, rotaryEndAngle, 0.6);

    g.fillPath(filledArc);

    g.setColour(Colour(0xff39587a).brighter());
    Path filledArc1;
    filledArc1.addPieSegment(rx, ry, rw + 1, rw + 1, rotaryStartAngle, angle, 0.6);

    g.fillPath(filledArc1);

    Path p;
    const float pointerLength = radius * 0.63f;
    const float pointerThickness = radius * 0.2f;
    p.addRectangle(-pointerThickness * 0.5f, -radius - 1, pointerThickness, pointerLength);
    p.applyTransform(AffineTransform::rotation(angle).translated(centreX, centreY));
    g.setColour(Colours::whitesmoke.darker());
    g.fillPath(p);

    const float dotradius = radius * (float)0.4;
    const float dotradius2 = rw * (float)0.4;
    g.setColour(Colours::whitesmoke.darker());
    g.fillEllipse(centreX - (dotradius),
        centreY - (dotradius),
        dotradius2, dotradius2);
}


/* SLIDER */
/* customise slider thumb appearance for FilterGUI */
void OtherLookAndFeelV1::drawRoundThumb (Graphics& g, float x, float y, float diameter, Colour colour, float outlineThickness)
{
    auto halfThickness = outlineThickness * 0.5f;

    Path p;
    p.addRoundedRectangle(x + halfThickness, y + halfThickness, diameter - outlineThickness, diameter - outlineThickness, 5.0f);

    DropShadow (Colours::black, 1, {}).drawForPath (g, p);

    g.setColour (Colour(0xff39587a).withMultipliedSaturation(0.2));
    g.fillPath (p);

//    g.setColour (colour.darker());
    g.strokePath (p, PathStrokeType (outlineThickness));
}


void OtherLookAndFeelV1::drawLinearSliderThumb (Graphics& g, int x, int y, int width, int height,
                                float sliderPos, float minSliderPos, float maxSliderPos,
                                const Slider::SliderStyle style, Slider& slider)
{
    auto sliderRadius = (float) (getSliderThumbRadius (slider) - 2);

    auto isDownOrDragging = slider.isEnabled() && (slider.isMouseOverOrDragging() || slider.isMouseButtonDown());

    auto knobColour = slider.findColour (Slider::thumbColourId)
                            .withMultipliedSaturation ((slider.hasKeyboardFocus (false) || isDownOrDragging) ? 1.3f : 0.9f)
                            .withMultipliedAlpha (slider.isEnabled() ? 1.0f : 0.7f);

    if (style == Slider::LinearHorizontal || style == Slider::LinearVertical)
    {
        float kx, ky;

        if (style == Slider::LinearVertical)
        {
            kx = (float) x + (float) width * 0.5f;
            ky = sliderPos;
        }
        else
        {
            kx = sliderPos;
            ky = (float) y + (float) height * 0.5f;
        }

        auto outlineThickness = slider.isEnabled() ? 0.8f : 0.3f;

        drawRoundThumb (g,
                        kx - sliderRadius,
                        ky - sliderRadius,
                        sliderRadius * 2.0f,
                        knobColour, outlineThickness);
    }
    else
    {
        // Just call the base class for the demo
        LookAndFeel_V2::drawLinearSliderThumb (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

/* customise slider appearance for FilterGUI */
void OtherLookAndFeelV1::drawLinearSlider (Graphics& g, int x, int y, int width, int height,
                           float sliderPos, float minSliderPos, float maxSliderPos,
                           const Slider::SliderStyle style, Slider& slider)
{
    g.fillAll (slider.findColour (Slider::backgroundColourId).withMultipliedBrightness(0.8f));

    if (style == Slider::LinearBar || style == Slider::LinearBarVertical)
    {
        Path p;

        if (style == Slider::LinearBarVertical)
            p.addRectangle ((float) x, sliderPos, (float) width, 1.0f + (float) height - sliderPos);
        else
            p.addRectangle ((float) x, (float) y, sliderPos - (float) x, (float) height);

        auto baseColour = slider.findColour (Slider::rotarySliderFillColourId)
                                .withMultipliedSaturation (slider.isEnabled() ? 1.0f : 0.5f)
                                .withMultipliedAlpha (0.8f);

        g.setColour (baseColour);
        g.fillPath (p);

        auto lineThickness = jmin (15.0f, (float) jmin (width, height) * 0.45f) * 0.1f;
        g.drawRect (slider.getLocalBounds().toFloat(), lineThickness);
    }
    else
    {
        drawLinearSliderBackground (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
        drawLinearSliderThumb      (g, x, y, width, height, sliderPos, minSliderPos, maxSliderPos, style, slider);
    }
}

/* customise slider background for FilterGUI */
void OtherLookAndFeelV1::drawLinearSliderBackground (Graphics& g, int x, int y, int width, int height,
                                     float /*sliderPos*/,
                                     float /*minSliderPos*/,
                                     float /*maxSliderPos*/,
                                     const Slider::SliderStyle /*style*/, Slider& slider)
{
    auto sliderRadius = (float) getSliderThumbRadius (slider) - 5.0f;
    Path on, off;

    if (slider.isHorizontal())
    {
        auto iy = (float) y + (float) height * 0.5f - sliderRadius * 0.5f;
        Rectangle<float> r ((float) x - sliderRadius * 0.5f, iy, (float) width + sliderRadius, sliderRadius);
        auto onW = r.getWidth() * ((float) slider.valueToProportionOfLength (slider.getValue()));

        on.addRectangle (r.removeFromLeft (onW));
        off.addRectangle (r);
    }
    else
    {
        auto ix = (float) x + (float) width * 0.5f - sliderRadius * 0.5f;
        Rectangle<float> r (ix, (float) y - sliderRadius * 0.5f, sliderRadius, (float) height + sliderRadius);
        auto onH = r.getHeight() * ((float) slider.valueToProportionOfLength (slider.getValue()));

        on.addRectangle (r.removeFromBottom (onH));
        off.addRectangle (r);
    }

//    g.setColour (slider.findColour (Slider::rotarySliderFillColourId).brighter());
    g.setColour(Colour(0xff39587a));
    g.fillPath (on);

    g.setColour (slider.findColour (Slider::trackColourId));
    g.fillPath (off);
}
