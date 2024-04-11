//
// Created by Max Ellis on 03/04/2024.
//

#ifndef FLANGER_DRAWNCUSTOMKNOB_H
#define FLANGER_DRAWNCUSTOMKNOB_H

#include "juce_audio_processors/juce_audio_processors.h"

namespace stompin_gui
{
    struct CustomSliderLnF : public juce::LookAndFeel_V4
    {
        void drawRotarySlider(juce::Graphics& g,
            int x,
            int y,
            int width,
            int height,
            float sliderPosProportional,
            float rotaryStartAngle,
            float rotaryEndAngle,
            juce::Slider& slider) override;
    };



    class DrawnCustomKnob : public juce::Slider
    {
    public:
        DrawnCustomKnob(juce::RangedAudioParameter& rap, const juce::String& unitSuffix, const juce::String& knobNameLabel, const juce::Font& fontToUse) :
           juce::Slider(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag, juce::Slider::TextEntryBoxPosition::NoTextBox),
           param(&rap),
           suffix(unitSuffix),
           nameLabel(knobNameLabel),
           font(fontToUse)
        {
            setLookAndFeel(&lnf);
        }

        ~DrawnCustomKnob() override
        {
            setLookAndFeel(nullptr);
        }

//        struct LabelPos
//        {
//            float pos;
//            juce::String label;
//        };
//
//        juce::Array<LabelPos> labels;

        void paint(juce::Graphics& g) override;

        juce::Rectangle<int> getSliderBounds() const;

        int getTextHeight() const {return 14;}

        juce::String getDisplayString() const;

        const juce::Font& getFont();


    private:
        stompin_gui::CustomSliderLnF lnf;
        juce::RangedAudioParameter* param;
        juce::String suffix, nameLabel;
        const juce::Font& font;
    };
}

#endif //FLANGER_DRAWNCUSTOMKNOB_H
