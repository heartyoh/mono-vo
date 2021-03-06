#include "common.h"

class Playback {

private:
  char folder[100];
  int seq;

public:
  int init();

  int capture(Mat& image);

  // parameters
  double getFocal();
  double getScale();
  Point2d getPrinciplePoint();
};
