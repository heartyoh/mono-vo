#include "common.h"

class Pose {

public:
  static int get(Pose& pose, const char* path, int idx);

  Pose(double x, double y, double z);
  Pose() {
    Pose(0, 0, 0);
  }

  double x;
  double y;
  double z;
};