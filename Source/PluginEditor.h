/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class IIRfilter_LPAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    IIRfilter_LPAudioProcessorEditor (IIRfilter_LPAudioProcessor&);
    ~IIRfilter_LPAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    IIRfilter_LPAudioProcessor& audioProcessor;
    
    juce::Slider cutoffSlider;
    juce::Slider qSlider;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> cutoffAttatchment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> qAttatchment;

    std::unique_ptr<juce::LookAndFeel_V4> lookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIRfilter_LPAudioProcessorEditor)
};
