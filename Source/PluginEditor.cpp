/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
IIRfilter_LPAudioProcessorEditor::IIRfilter_LPAudioProcessorEditor (IIRfilter_LPAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // LookAndFeel 설정
    auto lf = std::make_unique<juce::LookAndFeel_V4>();
    lf->setColourScheme(juce::LookAndFeel_V4::getGreyColourScheme());
    juce::LookAndFeel::setDefaultLookAndFeel(lf.get());
    lookAndFeel = std::move(lf);
    
    //CutoffSlider
    cutoffSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    cutoffSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(cutoffSlider);
    
    //Q factor
    qSlider.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    qSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
    addAndMakeVisible(qSlider);
    
    cutoffAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                        (audioProcessor.getApvts(), "Cutoff", cutoffSlider);
    qAttatchment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>
                        (audioProcessor.getApvts(), "Q factor", qSlider);
    
    setSize (400, 300);
}

IIRfilter_LPAudioProcessorEditor::~IIRfilter_LPAudioProcessorEditor()
{
}

//==============================================================================
void IIRfilter_LPAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (juce::FontOptions (30.0f));
    g.drawFittedText ("IIR LP Filter!", getLocalBounds().withTrimmedTop(30), juce::Justification::centredTop, 1);
}

void IIRfilter_LPAudioProcessorEditor::resized()
{
    cutoffSlider.setBounds(30, 100, 150, 150);
    qSlider.setBounds(220, 100, 150, 150);
}
