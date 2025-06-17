// calculate noise features of frame

#include <Arduino.h>
#include <sampling.h>

float rms(Frame f) {
  // calculate the root mean squared of a frame f
  // rms(f) = sqrt(sum(s[i]^2)/N)
  float *samples = f.samples;
  float output = 0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    output += samples[i] * samples[i];
  }
  output /= SAMPLE_COUNT;

  output = sqrt(output);

  return output;
}

float rlh(Frame f, float alpha) {
  // compute the ratio of low to high band energy
  // rlf = rms(fl)/rms(fh) where fl and fl are f after low/high pass filter
  float *samples = f.samples;
  Frame fl = {*f.samples};

  // apply low-pass filter to fl
  for (int i = 1; i < SAMPLE_COUNT; i++) {
    float sl_i1 = fl.samples[i - 1]; // fl[i-1]
    fl.samples[i] = sl_i1 + alpha * (samples[i] - sl_i1);
  }

  Frame fh = {*f.samples};
  // apply high-pass filter to fh
  for (int i = 1; i < SAMPLE_COUNT; i++) {
    float sh_i1 = fh.samples[i - 1];
    fh.samples[i] = alpha * (sh_i1 + samples[i] - samples[i - 1]);
  }

  return rms(fl) / rms(fh);
}

float std(Frame f) {
  // compute the standard deviation for a frame
}

float variance(Window w) {
  // compute the normalized std for a window
  // std_norm_i = ( std_i - std_mean ) / (std_mean - std_min)

  // 1. compute mean/min std
  // 2. iterate through frames and normalized std
}