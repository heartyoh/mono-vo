#include "params.hpp"

using namespace std;
using namespace cv;

double Params::getScaleFromDataset(int frame_id, int sequence_id, double z_cal, double* ox, double* oy, double* oz) {
  
  string line;
  int i = 0;
  ifstream myfile ("../data/01/poses.txt");
  double x =0, y=0, z = 0;
  double x_prev, y_prev, z_prev;
  if (myfile.is_open()) {
    while (( getline (myfile,line) ) && (i<=frame_id)) {
      z_prev = z;
      x_prev = x;
      y_prev = y;
      std::istringstream in(line);

      for (int j=0; j<12; j++) {
        in >> z ;
        if (j==7) y=z;
        if (j==3) x=z;
      }
      
      i++;
    }
    myfile.close();
  }

  else {
    cout << "Unable to open file";
    return 0;
  }

  *ox = x;
  *oy = y;
  *oz = z;

  return sqrt((x-x_prev)*(x-x_prev) + (y-y_prev)*(y-y_prev) + (z-z_prev)*(z-z_prev)) ;
}

double Params::getScale() {

  return 1.0;
}