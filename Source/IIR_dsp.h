/*
  ==============================================================================

    IIR_dsp.h
    Created: 30 Jul 2025 4:52:06pm
    Author:  sanghoonyou

  ==============================================================================
*/
#include <JuceHeader.h>
#pragma once
class IIR_dsp
{
    public:
        IIR_dsp();
        ~IIR_dsp();
        
        void prepare(const double inSampleRate);
        void updateCoefficients(const float inCutoff, const float Q);
        float process(float x);
        
    private:
        float x1, x2;
        float y1, y2;
        
        float a0, a1, a2;
        float b0, b1, b2;
        
        float fs;
        
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IIR_dsp)
};
