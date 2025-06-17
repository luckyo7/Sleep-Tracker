#ifndef SAMPLING_H
#define SAMPLING_H

// sample frames
#define SAMPLE_COUNT 1600 // number of samples per frame
#define SAMPLE_RATE 16000 // 16 kHz sample rate
#define WINDOW_SIZE 40    // number of frames per window

struct Frame {
  float samples[SAMPLE_COUNT];
  float rms;
  float rlh;
  float var; // normalized std within a window
};

struct Window {
  Frame *frames[WINDOW_SIZE];
};

void sampling_loop();
void i2s_install();
void i2s_setpin();

#endif