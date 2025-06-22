#include <sampling.h>
// calculate the sleep state and overall sleep efficiency for a night's sleep

#define MINUTE_LEN 20 // how many windows/minute

struct Minute {
  int eventCount; // # of total events for the minute
};

// TODO: store data as array of int, where each entry is one window, and int =
// #events

// better to agregate and store as one int, where its the sum of array

Minute countEvents(Window *windows[MINUTE_LEN]) {
  int count = 0;
  for (int i = 0; i < MINUTE_LEN; i++) {
    const Window &w = *windows[i];

    for (int k = 0; k < WINDOW_SIZE; k++) {
      count += w.frames[k]->event < EVENT_OTHER ? 1 : 0;
    }
  }

  return Minute{count}; // not sure if the scoping is right with this
}

const float coefficients[7] = {
    0.15, 0.15, 0.15, 0.08,
    0.21, 0.12, 0.13}; // see iSleep paper for info on these values

float calcSleepState(Minute *minutes[], int minutesLen, float coefficients[]) {
  // calculate the sleep state for a minute
  // computed as a weighted moving average with 4 frames behind and 2 ahead

  // minutesLen - # of minutes in the buffer
  float d = 0.0; // sleep state

  for (int i = 0; i < minutesLen; i++) {
    float c = coefficients[i]; // coefficient for weighted average
    Minute m = *minutes[i];
    d += c * m.eventCount;
  }

  return 0.125 * d;
};