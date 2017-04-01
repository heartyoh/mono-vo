#include "camera.hpp"

#define MAX_FRAME 151

int Camera::init() {
  sprintf(folder, "../data/01/images");
  seq = 0;
  return 0;
}

double Camera::getFocal() {
  return 718.8560;
}

double Camera::getScale() {

  if(seq < 3)
    return 1.0;

  string line;
  int i = 0;

  ifstream fposes ("../data/01/poses.txt");

  double x, y, z;
  double x_prev, y_prev, z_prev;
  
  if(!fposes.is_open()) {
    cout << "Unable to open file";
    return -1;
  }

  while(getline(fposes, line) && (i++ < seq - 3))
    ;

  while(getline(fposes, line) && (i++ <= seq - 1)) {

    z_prev = z;
    x_prev = x;
    y_prev = y;
    
    istringstream in(line);

    for (int j=0; j<12; j++) {
      in >> z ;
      if (j==7) y=z;
      if (j==3) x=z;
    }
  }

  fposes.close();

  return sqrt((x - x_prev)*(x - x_prev) + (y - y_prev)*(y - y_prev) + (z - z_prev)*(z - z_prev));
}

Point2d Camera::getPrinciplePoint() {
  return Point2d(607.1928, 185.2157);
}

int Camera::capture(Mat& image) {

  if(seq >= MAX_FRAME)
    return -1;

  char filename[200];
  Mat grayImage;

  sprintf(filename, "%s/%06d.png", folder, seq);
  Mat colorImage = imread(filename);

  cvtColor(colorImage, grayImage, COLOR_BGR2GRAY);

  image = grayImage;

  return seq++;
}