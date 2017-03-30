#include "common.h"

class Plotter {

protected:
  Mat trajectory;

public:

  Plotter();
  int plot(double dx, double dy, double dz, double ox, double oy, double oz);
};