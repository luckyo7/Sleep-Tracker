#ifndef NOISE_H
#define NOISE_H
// handle noise model and related calculations

struct NoiseModel {
  float meanRMS;
  float meanRLH;
  float meanVar;

  float stdRMS;
  float stdRLH;
  float stdVar;
};

extern NoiseModel noiseModel;

#endif