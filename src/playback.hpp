#include "common.h"

class Playback {

  char name[100];

public:
  Playback(const char* name);

  int redraw(Mat image);
};