//
// Created by Max Ellis on 06/04/2024.
//

#include "CustomToggleButtons.h"

void stompin_gui::CustomToggleLnF::drawToggleButton(juce::Graphics& g,
    juce::ToggleButton& toggleButton,
    bool shouldDrawButtonAsHighlighted,
    bool shouldDrawButtonAsDown)
{
    auto bounds = toggleButton.getLocalBounds().reduced(5.f);
    auto size = juce::jmin(bounds.getWidth(), bounds.getHeight());
    auto r = bounds.withSizeKeepingCentre(size, size);

    auto* tb = dynamic_cast<CustomToggleButtons*> (&toggleButton);

    if (!tb->noteButton)
    {
        if (toggleButton.getToggleState())
        {
            g.setColour (juce::Colours::black);
            g.fillRect (r);
            g.setColour(juce::Colours::dimgrey);
            g.drawRect(r);
            g.setColour (juce::Colours::white);
            g.setFont (tb->getFont());
            g.drawFittedText ("Hz", r, juce::Justification::centred, 1);
        }
        else
        {
            g.setColour (juce::Colours::white);
            g.setOpacity(0);
            g.fillRect (r);
            g.setColour(juce::Colours::dimgrey);
            g.drawRect(r);
            g.setColour (juce::Colours::black);
            g.setFont (tb->getFont());
            g.drawFittedText ("Hz", r, juce::Justification::centred, 1);
        }
    }
    else
    {
        if (toggleButton.getToggleState())
        {
            g.setColour (juce::Colours::black);
            g.fillRect (r);
            g.setColour(juce::Colours::dimgrey);
            g.drawRect(r);
            g.setColour (juce::Colours::white);
            g.fillEllipse(r.getX() + r.getWidth() * 0.35f, r.getCentreY() + r.getWidth() * 0.1f, r.getWidth() * 0.25f, r.getHeight() * 0.2f);
            auto stalk = juce::Rectangle<float>(r.getX() + r.getWidth() * 0.5f, r.getY() + r.getHeight() * 0.2f, r.getWidth() * 0.1f, r.getHeight() * 0.45f);
            g.fillRect(stalk);
        }
        else
        {
            g.setColour (juce::Colours::white);
            g.setOpacity(0);
            g.fillRect (r);
            g.setColour(juce::Colours::dimgrey);
            g.drawRect(r);
            g.setColour (juce::Colours::black);
            g.fillEllipse(r.getX() + r.getWidth() * 0.35f, r.getCentreY() + r.getWidth() * 0.1f, r.getWidth() * 0.25f, r.getHeight() * 0.2f);
            auto stalk = juce::Rectangle<float>(r.getX() + r.getWidth() * 0.5f, r.getY() + r.getHeight() * 0.2f, r.getWidth() * 0.1f, r.getHeight() * 0.45f);
            g.fillRect(stalk);
        }
    }

//    note button

}

void stompin_gui::CustomToggleButtons::paint (juce::Graphics& g)
{
    getLookAndFeel().drawToggleButton(g, *this, false, false);
}

const juce::Font& stompin_gui::CustomToggleButtons::getFont()
{
    return font;
}