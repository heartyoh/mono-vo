#include "playback.hpp"

Playback::Playback(const char* name) {
  bzero(this->name, 100);
  strncpy(this->name, name, strlen(name));
  namedWindow(this->name, WINDOW_AUTOSIZE);

  AXIS = Mat(3, 3, CV_64F);

  AXIS.at<double>(0, 0) = 1;
  AXIS.at<double>(1, 0) = 0;
  AXIS.at<double>(2, 0) = 0;
  
  AXIS.at<double>(0, 1) = 0;
  AXIS.at<double>(1, 1) = 1;
  AXIS.at<double>(2, 1) = 0;
  
  AXIS.at<double>(0, 2) = 0;
  AXIS.at<double>(1, 2) = 0;
  AXIS.at<double>(2, 2) = 1;
  
}

int Playback::redraw(Mat image, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures, Mat& R) {

  if(prevFeatures.size() == currFeatures.size()) {

    for(int i = 0;i < prevFeatures.size();i++) {
      Point2f prev = prevFeatures.at(i);
      Point2f curr = currFeatures.at(i);
      
      line(image, prev, curr, CV_RGB(0, 255, 0));
    }
  }

  if(R.rows > 0 && R.cols > 0) {
    
    Point2f origin(600, 50);

    double *r = (double*)R.data;

    AXIS = R * AXIS;
    double *axis = (double*)AXIS.data;

    Point2f xAxis(axis[0] * 100 + origin.x, axis[3] * 100 + origin.y);
    Point2f yAxis(axis[1] * 100 + origin.x, axis[4] * 100 + origin.y);
    Point2f zAxis(axis[2] * 100 + origin.x, axis[5] * 100 + origin.y);

    line(image, origin, xAxis, CV_RGB(255, 0, 0), 2);
    line(image, origin, yAxis, CV_RGB(0, 255, 0), 2);
    line(image, origin, zAxis, CV_RGB(0, 0, 255), 2);
  }

  imshow(name, image);

  return 0;
}
