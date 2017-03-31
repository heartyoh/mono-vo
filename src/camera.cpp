#include "camera.hpp"

#define MAX_FRAME 151

int Camera::init() {
  sprintf(folder, "../data/01/images");
  seq = 0;
  return 0;
}

int Camera::capture(Mat& image) {

  if(seq >= MAX_FRAME)
    return -1;

  char filename[200];

  sprintf(filename, "%s/%06d.png", folder, seq);
  image = imread(filename);

  return seq++;
}