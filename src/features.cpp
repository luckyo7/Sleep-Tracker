// calculate noise features of frame
// normalize the features for a window too

#include <Arduino.h>
#include <noiseModel.h>
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

float mean(const Frame &f) {
  const float *samples = f.samples;
  float mean = 0.0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    mean += samples[i];
  }

  mean /= SAMPLE_COUNT;

  return mean;
}

float std(const Frame &f) {
  // compute the standard deviation for a frame
  // std = sqrt(1/N*sum((s_i - mean)^2))

  const float *samples = f.samples;
  float std = 0.0;
  float fMean = mean(f);
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    std += (samples[i] - fMean) * (samples[i] - fMean);
  }
  std /= SAMPLE_COUNT;

  std = sqrt(std);

  return std;
}

void computeVariance(Window &w) {
  // compute the normalized std for a window
  // modify in place
  // also computes std
  // std_norm_i = ( std_i - std_mean ) / (std_mean - std_min)

  // 1. compute mean/min std
  // 2. iterate through frames and normalize std
  float std_mean = 0.0;
  float std_min = 1000.0; // sorry for all the pythonic code it just looks some
                          // much nicer to me
  float std_arr[WINDOW_SIZE];
  for (int i = 0; i < WINDOW_SIZE; i++) {
    Frame *f = w.frames[i];
    float stdF = std(*f); // first time dereferencing a pointer!
    f->std = stdF;
    std_arr[i] = stdF;

    std_mean += stdF;

    if (stdF < std_min) {
      std_min = stdF;
    }
  }

  std_mean /= WINDOW_SIZE;

  // could I do all of this in one for loop?
  // not sure how to deal with mean
  // rearrange with algebra?

  // normalize std_i
  for (int i = 0; i < WINDOW_SIZE; i++) {
    Frame *f = w.frames[i];

    float std_i = std_arr[i];

    float denom = std_mean - std_min;
    denom = denom < 1e-6 ? 1.0 : denom; // use 1.0 if < 1e-6; avoid div 0 error

    float std_norm = (std_i - std_mean) /
                     (std_mean - std_min); // normalized std; see iSleep paper

    // modify in place
    f->var = std_norm; // pointer syntax is so weird looking
  }
}

void normalizeFeatures(Window &w) {
  // normalize the features for each frame in a window w
  // modified in place

  // Noise model values
  float meanRMS = noiseModel.meanRMS;
  float stdRMS = noiseModel.stdRMS;

  float meanRLH = noiseModel.meanRLH;
  float stdRLH = noiseModel.stdRLH;

  float meanVar = noiseModel.meanVar;
  float stdVar = noiseModel.stdVar;
  // Noise model values

  // iterate through frames to modify in place
  for (int i = 0; i < WINDOW_SIZE; i++) {
    Frame *f = w.frames[i];

    float frameRMS = f->rms;
    float frameRLH = f->rlh;
    float frameVar = f->var;

    float normRMS = (frameRMS - meanRMS) / stdRMS;
    float normRLH = (frameRLH - meanRLH) / stdRLH;
    float normVar = (frameVar - meanVar) / stdVar;

    f->rms = normRMS;
    f->rlh = normRLH;
    f->var = normVar;
  }
}