#include "camera.hpp"

#define MAX_FRAME 100000

void Camera::init() {
  this->init(0);
}

void Camera::init(int cameraNumber) {
  seq = 0;
  // Get access to the default camera.
  try {
    camera.open(cameraNumber);
    sleep(1);
  } catch (Exception e) {}
  if ( !camera.isOpened() ) {
    cerr << "ERROR: Could not access the camera!" << endl;
    exit(1);
  }
  cout << "Loaded camera " << cameraNumber << "." << endl;

  camera.set(CV_CAP_PROP_FRAME_WIDTH, 640);
  camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
  camera.set(CV_CAP_PROP_AUTOFOCUS, 0);
}

double Camera::getFocal() {
  // return 718.8560;
  return 514.7320;
}

double Camera::getScale() {

  return 1.0;
}

Point2d Camera::getPrinciplePoint() {
  // return Point2d(607.1928, 185.2157);
  return Point2d(335.7723, 247.8904);
}

int Camera::capture(Mat& image) {
  camera >> cameraFrame;
  if(cameraFrame.empty()){
    cerr << "ERROR: Couldn't grab the next camera frame." << endl;
    exit(1);
  }

  Mat grayImage;

  cvtColor(cameraFrame, grayImage, COLOR_BGR2GRAY);

  image = grayImage;

  return seq++;
}
