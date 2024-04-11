#include "PluginEditor.h"

PluginEditor::PluginEditor (PluginProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p), customFont(getCustomFont()),
      delay(*processorRef.apvts.getParameter("delay"), "ms", "Delay", customFont),
      rateNotes(*processorRef.apvts.getParameter("rateNotes"), "", "LFO Rate", customFont),
      depth(*processorRef.apvts.getParameter("depth"), "%", "Depth", customFont),
      mix(*processorRef.apvts.getParameter("mix"), "", "Mix", customFont),
      feedback(*processorRef.apvts.getParameter("feedback"), "%", "Feedback", customFont),
      rate(*processorRef.apvts.getParameter("rate"), "Hz", "LFO Rate", customFont),
      delayAttach(processorRef.apvts, "delay", delay),
      rateNotesAttach(processorRef.apvts, "rateNotes", rateNotes),
      depthAttach(processorRef.apvts, "depth", depth),
      mixAttach(processorRef.apvts, "mix", mix),
      feedbackAttach(processorRef.apvts, "feedback", feedback),
      rateAttach(processorRef.apvts, "rate", rate),
      noteButton(customFont, true),
      hzButton(customFont, false),
      noteButtonAttach(processorRef.apvts, "noteRateButton", noteButton),
      hzButtonAttach(processorRef.apvts, "hzRateButton", hzButton)

{
    stompinLogo = juce::ImageCache::getFromMemory(BinaryData::StompinLogo_png, BinaryData::StompinLogo_pngSize);

    addAndMakeVisible(rateNotes);
    addAndMakeVisible(depth);
    addAndMakeVisible(mix);
    addAndMakeVisible(feedback);
    addAndMakeVisible(delay);
    addAndMakeVisible(noteButton);
    addAndMakeVisible(hzButton);

    noteButton.onClick = [this](){
        noteButton.setToggleState(true, juce::dontSendNotification);
        hzButton.setToggleState(false, juce::dontSendNotification);
        addAndMakeVisible(rateNotes);
        removeChildComponent(&rate);
        processorRef.notes = true;
    };

    hzButton.onClick = [this](){
        hzButton.setToggleState(true, juce::dontSendNotification);
        noteButton.setToggleState(false, juce::dontSendNotification);
        addAndMakeVisible(rate);
        removeChildComponent(&rateNotes);
        processorRef.notes = false;
    };


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (700, 175);
}

PluginEditor::~PluginEditor()
{

}

void PluginEditor::paint (juce::Graphics& g)
{
    juce::Image background = juce::ImageCache::getFromMemory(BinaryData::backgroiund_jpeg, BinaryData::backgroiund_jpegSize);
    g.drawImageAt(background, 0, 0);
}

void PluginEditor::resized()
{
    // layout the positions of your child components here
    auto bounds = getLocalBounds();
    auto buttonBounds = bounds.removeFromLeft(bounds.proportionOfWidth(0.05));
    buttonBounds = buttonBounds.reduced(0, buttonBounds.proportionOfHeight(0.2));
    noteButton.setBounds(buttonBounds.removeFromTop(buttonBounds.proportionOfHeight(0.5)));
    hzButton.setBounds(buttonBounds);
    auto rateBounds = bounds.removeFromLeft(bounds.proportionOfWidth(0.2));
    rateNotes.setBounds(rateBounds);
    rate.setBounds(rateBounds);
    depth.setBounds(bounds.removeFromLeft(bounds.proportionOfWidth(0.25)));
    delay.setBounds(bounds.removeFromLeft(bounds.proportionOfWidth(0.335)));
    feedback.setBounds(bounds.removeFromLeft(bounds.proportionOfWidth(0.5)));
    mix.setBounds(bounds);
}

juce::Font PluginEditor::getCustomFont()
{
    static auto typeFace = juce::Typeface::createSystemTypefaceFor(BinaryData::Harmonia_Sans_W01_Bold_ttf, BinaryData::Harmonia_Sans_W01_Bold_ttfSize);
    return typeFace;
}
