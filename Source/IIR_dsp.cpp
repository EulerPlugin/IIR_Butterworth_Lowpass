/*
  ==============================================================================

    IIR_dsp.cpp
    Created: 30 Jul 2025 4:52:06pm
    Author:  sanghoonyou

  ==============================================================================
*/
#include "IIR_dsp.h"

IIR_dsp::IIR_dsp()
:x1(0.0f), x2(0.0f),
y1(0.0f), y2(0.0f),
a0(0.0f), a1(0.0f), a2(0.0f),
b0(0.0f), b1(0.0f), b2(0.0f),
fs(44100.0f)
{
    
}

IIR_dsp::~IIR_dsp()
{
    
}

void IIR_dsp::prepare(const double inSampleRate)
{
    fs = inSampleRate;
}

void IIR_dsp::updateCoefficients(const float inCutoff, const float Q)
{
   const float omega = juce::MathConstants<float>::pi * inCutoff / fs;
   
   const float k = std::tan(omega);
   
   const float norm = 1.0f / (1.0f + k / Q + k * k);
   
   a0 = 1.0f;
   
   a1 = (2 * k * k - 2) * norm;
   
   a2 = (1 - k/Q + k * k) * norm;
   
   b0 = (k * k) * norm;
   
   b1 = (2 * k * k) * norm;
   
   b2 = (k * k) * norm;
   
}

float IIR_dsp::process(float x)
{
    float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    
    x2 = x1;
    x1 = x;
    
    y2 = y1;
    y1 = y;
    
    return y;
}
