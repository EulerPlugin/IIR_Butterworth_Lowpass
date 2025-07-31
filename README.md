# IIR Butterworth Low-Pass Filter
<p float="left">
  <img src="https://github.com/user-attachments/assets/51ace967-8fbe-43be-8095-64b98d592c71" width="45%" />
  <img src="https://github.com/user-attachments/assets/848f6c1f-5589-49ab-aacd-fb6794820361" width="45%" />
</p>


<br>


### Properties of the Butterworth filter

- The Butterworth filter is designed to be maximally flat in the passband
- As a result, its magnitude response is monotonic,so it has no ripples
- This smooth behavior is a desirable property for filters used in music and audio processing


<br>


## Butterworth Filter Design Process

### 1. Specify the filter in the digital domain
- a lowpass filter
- of order 2
- with a controllable cutoff frequency fc in Hz
- with a controllable quality factor Q.


<br>


### 2. Decide on the analog prototype with a transfer function H(s)

The next step is to pick the analog prototype that we will digitize. As we said, in this example, we are choosing the Butterworth lowpass of order 2 with the already presented transfer function.

<p align="center">
  <img src="https://github.com/user-attachments/assets/a224b883-b181-4917-876e-a64cc2ec2268" width="268" height="88" alt="image" />
</p>

<br>


### 3. Prewarp the cutoff frequency to counteract the warping in the bilinear transformation

The bilinear transform warps frequencies, so the digital filter's cutoff won't match the analog one. To fix this, we prewarp the analog cutoff so it maps correctly after digitization.

<p align="center">
<img width="268" height="88" alt="image" src="https://github.com/user-attachments/assets/9c368d09-3242-4c40-861f-66e38e905606" />
</p>

~~~cpp
void IIR_dsp::updateCoefficients(const float inCutoff, const float Q)
{
   const float omega = juce::MathConstants<float>::pi * inCutoff / fs;
}
~~~

<br>

### 4. Perform the bilinear transformation

The bilinear transformation is a mapping of the s-plane to the z-plane expressed by the formula

<p align="center">
<img width="268" height="88" alt="image" src="https://github.com/user-attachments/assets/5fb1a323-33d7-4b3e-b2e5-6fe2fde5d535" />
</p>

<br>


### 5.Reorganize the digital transfer function to get the filter coefficients or the difference equation

<p align="center">
<img width="234" height="73" alt="image" src="https://github.com/user-attachments/assets/83483a3b-3e9f-4c51-968b-b5356b683bad" />
</p>

~~~cpp
void IIR_dsp::updateCoefficients(const float inCutoff, const float Q)
{
   const float k = std::tan(omega);  
}
~~~

<br>


A digital filter is represented by the following transfer function form:

<p align="center">
<img width="303" height="77" alt="image" src="https://github.com/user-attachments/assets/2858413d-9745-4780-9cf1-b8be4164f43e" />
</p>

<p align="center">
<img width="197" height="221" alt="image" src="https://github.com/user-attachments/assets/60538aa8-ba93-4552-a47d-cff5522bac42" />
</p>

However, However, all coefficients are divided by a0, undergoing a normalization process.

~~~cpp
   const float norm = 1.0f / (1.0f + k / Q + k * k);
   
   a0 = 1.0f;
   
   a1 = (2 * k * k - 2) * norm;
   
   a2 = (1 - k/Q + k * k) * norm;
   
   b0 = (k * k) * norm;
   
   b1 = (2 * k * k) * norm;
   
   b2 = (k * k) * norm;
~~~

<br>

### 6. Second-order IIR difference equation

<p align="center">
<img width="550" height="48" alt="image" src="https://github.com/user-attachments/assets/8563013f-d6ab-4689-b3ae-1bfdb4d268f3" />
</p>

~~~cpp
float IIR_dsp::process(float x)
{
    float y = b0 * x + b1 * x1 + b2 * x2 - a1 * y1 - a2 * y2;
    
    x2 = x1;
    x1 = x;
    
    y2 = y1;
    y1 = y;
    
    return y;
}
~~~
