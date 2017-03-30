#include "common.h"

class Logger {

  ofstream file;

public:
  Logger(const char* filename);
  int log(double dx, double dy, double dz);

};
