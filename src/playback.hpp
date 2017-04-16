#include "common.h"

class Playback {

  char name[100];
  Mat AXIS;

public:
  Playback(const char* name);

  int redraw(Mat image, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures, Mat& R);
};