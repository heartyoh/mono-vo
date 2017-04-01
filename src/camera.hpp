#include "common.h"

class Camera {

private:
  char folder[100];
  int seq;

public:
  int init();

  int capture(Mat& image);

  // parameters
  double getFocal();
  double getScale();
  double getScaleFromDataset();
  Point2d getPrinciplePoint();
};