//
// Created by Max Ellis on 06/04/2024.
//

#ifndef FLANGER_CUSTOMTOGGLEBUTTONS_H
#define FLANGER_CUSTOMTOGGLEBUTTONS_H

#include "juce_gui_basics/juce_gui_basics.h"

namespace stompin_gui
{
    struct CustomToggleLnF : public juce::LookAndFeel_V4
    {
        void drawToggleButton(juce::Graphics& g,
            juce::ToggleButton& toggleButton,
            bool shouldDrawButtonAsHighlighted,
            bool shouldDrawButtonAsDown) override;
    };

    class CustomToggleButtons : public juce::ToggleButton
    {
    public:
        CustomToggleButtons(const juce::Font& fontToUse, bool note) : noteButton(note), font(fontToUse)
        {
            setLookAndFeel(&lnf);
        }

        ~CustomToggleButtons() override
        {
            setLookAndFeel(nullptr);
        }

        const juce::Font& getFont();

        bool noteButton;

        void paint(juce::Graphics& g) override;

    private:
        CustomToggleLnF lnf;
        const juce::Font& font;

    };
}

#endif //FLANGER_CUSTOMTOGGLEBUTTONS_H
