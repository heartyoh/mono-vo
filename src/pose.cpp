#include "pose.hpp"

Pose::Pose(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

int Pose::get(Pose& pose, const char* path, int idx) {
  
  string line;
  int i = 0;
  double x, y, z;

  ifstream fposes(path);
  if(!fposes.is_open()) {
    cout << "Unable to open file";
    return -1;
  }

  while(i++ <= idx)
    getline(fposes, line);

  istringstream in(line);

  for(int j = 0;j < 12;j++) {
    in >> z;
    if(j == 7) 
      y = z;
    if(j == 3)
      x = z;
  }

  fposes.close();

  pose = Pose(x, y, z);

  return 0;
}
