#ifndef SAMPLING_H
#define SAMPLING_H

// sample frames
#define SAMPLE_COUNT 1600 // number of samples per frame
#define SAMPLE_RATE 16000 // 16 kHz sample rate
#define WINDOW_SIZE 40    // number of frames per window

enum Event {
  EVENT_SNORE, // snoring event
  EVENT_COUGH, // coughing event
  EVENT_MOVE,  // moving around, tossing/turning
  EVENT_OTHER, // frame is noise
  EVENT_ERROR  // some error, likely frame has not been normalized or it is
               // uninitialized
};

struct Frame {
  float samples[SAMPLE_COUNT];
  float rms;
  float rlh;
  float std;
  float var;             // normalized std within a window
  bool isNormal = false; // has the frame been normalized yet
  Event event = EVENT_ERROR;

  Frame(float samples[]) { // I hate that we have to manually copy arrays in C++
    for (int i = 0; i < SAMPLE_COUNT; i++) {
      this->samples[i] = samples[i];
    }
  };
};

struct Window {
  Frame *frames[WINDOW_SIZE];
  bool isNormal;
};

void sampling_loop();
void i2s_install();
void i2s_setpin();

#endif