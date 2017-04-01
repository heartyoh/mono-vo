#include "common.h"
#include "pose.hpp"

class Plotter {

protected:
  Mat trajectory;

public:

  Plotter();
  int plot(double dx, double dy, double dz, Pose truth);
};