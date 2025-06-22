#include <Arduino.h>
#include <sampling.h>
// handle decision tree logic for event characterization

float tRLH = 8.0; // threshold for the first step (RLH)

float tVar1 = 0.1;   // threshold for initial var check
float tVar2 = 450.0; // threshold for var check for snoring

float tRMS = 9.0; // threshold for RMS

Event classifyEvent(const Frame &f) {
  if (!f.isNormal) {
    Serial.println("Frame is not normalized!");
    return EVENT_ERROR;
  }

  // we use three steps to classify:
  // 1. rlh > 8
  // 2. var < 0.1
  // 3. rms > 9

  // step 1.
  if (f.rlh > tRLH) {
    // either snore or other
    if (f.var >= tVar2) {
      // other
      return EVENT_OTHER;
    } else {
      return EVENT_SNORE;
    }
  } else if (f.var >= tVar1) {
    // cough
    return EVENT_COUGH;
  } else if (f.rms > tRMS) {
    // move
    return EVENT_MOVE;
  }
  return EVENT_OTHER;
}