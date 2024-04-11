//
// Created by Max Ellis on 03/04/2024.
//

#include "DrawnCustomKnob.h"

void stompin_gui::DrawnCustomKnob::paint(juce::Graphics &g)
{
    using namespace juce;

    auto startAng = degreesToRadians(180.f + 40.f);
    auto endAng = degreesToRadians(180.f - 40.f) + MathConstants<float>::twoPi;
    auto range = getRange();
    auto nameLabelBounds = getLocalBounds().removeFromTop(getLocalBounds().proportionOfHeight(0.2));

    g.setFont (font);
    g.setColour (juce::Colours::black);
    g.drawFittedText (nameLabel, nameLabelBounds, juce::Justification::centred, 1);

    auto sliderBounds = getSliderBounds();

//    g.setColour(Colours::red);
//    g.drawRect(getLocalBounds());
//    g.setColour(Colours::yellow);
//    g.drawRect(sliderBounds);


    getLookAndFeel().drawRotarySlider(g,
        sliderBounds.getX(),
        sliderBounds.getY(),
        sliderBounds.getWidth(),
        sliderBounds.getHeight(),
        jmap(getValue(),
            range.getStart(),
            range.getEnd(),
            0.0,
            1.0),
        startAng,
        endAng,
        *this);

    auto center = sliderBounds.toFloat().getCentre();
    auto radius = sliderBounds.getWidth() * 0.5f;

    g.setColour(Colour(223u, 223u, 224u));
    g.setFont(getTextHeight());

//    auto numChoices = labels.size();
//    for(int i=0; i<numChoices; ++i)
//    {
//        auto pos = labels[i].pos;
//        jassert(0.f <= pos);
//        jassert(pos <= 1.f);
//
//        auto ang = jmap(pos, 0.f, 1.f, startAng, endAng);
//
//        auto c = center.getPointOnCircumference(radius + getTextHeight() * 0.5f + 1, ang);
//
//        Rectangle<float> r;
//        auto str = labels[i].label;
//        r.setSize(g.getCurrentFont().getStringWidth(str), getTextHeight());
//        r.setCentre(c);
//        r.setY(r.getY() + getTextHeight());
//
//        g.drawFittedText(str, r.toNearestInt(), juce::Justification::centred, 1);

//    }
}

juce::Rectangle<int> stompin_gui::DrawnCustomKnob::getSliderBounds() const
{
    auto bounds = getLocalBounds();

    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());

    size -= getTextHeight() * 2;
    juce::Rectangle<int> r;
    r.setSize(size, size);
    r.setCentre(bounds.getCentreX(), bounds.getCentreY());

    return r;
}

juce::String stompin_gui::DrawnCustomKnob::getDisplayString() const
{
    //return juce::String(getValue());
    if(auto* choiceParam = dynamic_cast<juce::AudioParameterChoice*>(param))
    {
        return choiceParam->getCurrentChoiceName();
    }

    juce::String str;
    bool addK = false;
    auto range = getRange();

    if(auto* floatParam = dynamic_cast<juce::AudioParameterFloat*>(param))
    {
        float val = getValue();
        if(val>999.f)
        {
            val /= 1000.f;
            addK = true;
        }
        else if(range.getStart() >= -1 && range.getEnd() <= 1)
        {
            val *= 100.f;
        }
        str = juce::String(val, (addK ? 2 : 0));
    }
    else
    {
        jassertfalse; //this shouldn't happen!!
    }

    if(suffix.isNotEmpty())
    {
        str << " ";
        if(addK)
        {
            str << "k";
        }
        str << suffix;
    }
    return str;
}

const juce::Font& stompin_gui::DrawnCustomKnob::getFont()
{
    return font;
}



void stompin_gui::CustomSliderLnF::drawRotarySlider(juce::Graphics& g,
    int x,
    int y,
    int width,
    int height,
    float sliderPosProportional,
    float rotaryStartAngle,
    float rotaryEndAngle,
    juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float> (x, y, width, height);

    auto enabled = slider.isEnabled();

    g.setColour (juce::Colours::black);
    g.drawEllipse (bounds.reduced (8.f), 5.f); //internal knob

    if (auto* rswl = dynamic_cast<stompin_gui::DrawnCustomKnob*> (&slider))
    {
        auto center = bounds.getCentre();

        juce::Path p, p2;

        juce::Rectangle<float> r;
        r.setLeft (center.getX() - 2);
        r.setRight (center.getX() + 2);
        r.setTop (bounds.reduced (8.f).getY());
        r.setBottom (center.getY() * 0.75);

        p.addRoundedRectangle (r, 2.f);

        jassert (rotaryStartAngle < rotaryEndAngle);

        auto sliderAngRad = juce::jmap (sliderPosProportional, 0.f, 1.f, rotaryStartAngle, rotaryEndAngle);

        p2.addCentredArc (center.getX(), center.getY(), bounds.reduced (25.f).getWidth(), bounds.reduced (25.f).getHeight(), juce::degreesToRadians (284), rotaryStartAngle, sliderAngRad, true);

        p.applyTransform (juce::AffineTransform().rotated (sliderAngRad, center.getX(), center.getY()));

        g.fillPath (p);
        g.strokePath (p2, juce::PathStrokeType (5.f, juce::PathStrokeType::JointStyle::curved, juce::PathStrokeType::EndCapStyle::rounded));

        g.setFont (rswl->getFont());
        auto text = rswl->getDisplayString();
        auto strWidth = g.getCurrentFont().getStringWidth (text);

        r.setSize (strWidth + 4, rswl->getTextBoxHeight() + 2);
        r.setCentre (center);

        g.setColour (juce::Colours::black);
        g.drawFittedText (text, r.toNearestInt(), juce::Justification::centred, 1);
    }
}