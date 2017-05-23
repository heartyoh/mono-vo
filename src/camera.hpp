#include "common.h"

class Camera {
private:
  VideoCapture camera;
  Mat cameraFrame;
  int seq;

public:
  void init();
  void init(int cameraNumber);

  int capture(Mat &image);

  double getFocal();
  double getScale();
  Point2d getPrinciplePoint();
};
