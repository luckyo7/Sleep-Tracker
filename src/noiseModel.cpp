#include <Arduino.h>
#include <noiseModel.h>
#include <sampling.h>
// handle noise model and related calculations
// noise model needs to store: mean(rms) and std(rms)
float std(float arr[], int arrLen, float arrMean) {
  // compute the standard deviation for a frame
  // std = sqrt(1/N*sum((s_i - mean)^2))

  float std = 0.0;
  for (int i = 0; i < SAMPLE_COUNT; i++) {
    std += (arr[i] - arrMean) * (arr[i] - arrMean);
  }
  std /= SAMPLE_COUNT;

  std = sqrt(std);

  return std;
}

float mean(float arr[], int arrLen) {
  float mean = 0.0;

  for (int i = 0; i < arrLen; i++) {
    mean += arr[i];
  }

  return mean;
}

NoiseModel noiseModel = {0.0, 0.0}; // current noise model
void updateNoiseModel(const Frame *sequence[], int sequenceLength,
                      NoiseModel &model) {
  // update the noise model with a section of frames
  // modify the model in place
  // rms += 0.5(rms(f) - rms)
  float rmsArr[sequenceLength] = {0};
  float rlhArr[sequenceLength] = {0};
  float varArr[sequenceLength] = {0};

  for (int i = 0; i < sequenceLength; i++) {
    const Frame *f = sequence[i];
    rmsArr[i] = f->rms;
    rlhArr[i] = f->rlh;
    varArr[i] = f->var;
  }

  float meanRMS = mean(rmsArr, sequenceLength);
  float meanRLH = mean(rlhArr, sequenceLength);
  float meanVar = mean(varArr, sequenceLength);

  float stdRMS = std(rmsArr, sequenceLength, meanRMS);
  float stdRLH = std(rlhArr, sequenceLength, meanRLH);
  float stdVar = std(varArr, sequenceLength, meanVar);
  // model.meanRMS += 0.5 * (f.rms - model.meanRMS);
  // model.stdRMS += 0.5 * (f.std - model.stdRMS);

  model.meanRMS += 0.5 * (meanRMS - model.meanRMS);
  model.meanRLH += 0.5 * (meanRLH - model.meanRLH);
  model.meanVar += 0.5 * (meanVar - model.meanVar);

  model.stdRMS += 0.5 * (stdRMS - model.stdRMS);
  model.stdRLH += 0.5 * (stdRLH - model.stdRLH);
  model.stdVar += 0.5 * (stdVar - model.stdVar);
}