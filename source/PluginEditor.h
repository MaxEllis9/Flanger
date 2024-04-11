#pragma once

#include "PluginProcessor.h"
#include "BinaryData.h"
#include "DrawnCustomKnob.h"
#include "CustomToggleButtons.h"

//==============================================================================
class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor (PluginProcessor&);
    ~PluginEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    juce::Font getCustomFont();

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    PluginProcessor& processorRef;
    const juce::Font customFont;
    stompin_gui::DrawnCustomKnob delay, rateNotes, depth, mix, feedback, rate;
    juce::AudioProcessorValueTreeState::SliderAttachment delayAttach, rateNotesAttach, depthAttach, mixAttach, feedbackAttach, rateAttach;
    stompin_gui::CustomToggleButtons noteButton, hzButton;
    juce::AudioProcessorValueTreeState::ButtonAttachment noteButtonAttach, hzButtonAttach;
    juce::Image stompinLogo;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginEditor)
};
