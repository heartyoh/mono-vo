#include "preview.hpp"

Preview::Preview(const char* name) {
  bzero(this->name, 100);
  strncpy(this->name, name, strlen(name));
  namedWindow(this->name, WINDOW_AUTOSIZE);
  this->fontFace = FONT_HERSHEY_PLAIN;
  this->fontScale = 1;

  AXIS = (Mat_<double>(3,3) << 1,0,0, 0,1,0, 0,0,1);
}

int Preview::redraw(Mat image, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures, Mat& R) {

  if(prevFeatures.size() == currFeatures.size()) {

    for(int i = 0;i < prevFeatures.size();i++) {
      Point2f prev = prevFeatures.at(i);
      Point2f curr = currFeatures.at(i);

      line(image, prev, curr, CV_RGB(0, 255, 0));
    }
  }

  if(R.rows > 0 && R.cols > 0) {

    Point2f origin(500, 100);

    double *r = (double*)R.data;

    Point2f xAxis(r[0] * 100 + origin.x, r[3] * 100 + origin.y);
    Point2f yAxis(r[1] * 100 + origin.x, r[4] * 100 + origin.y);
    Point2f zAxis(r[2] * 100 + origin.x, r[5] * 100 + origin.y);

    line(image, origin, xAxis, CV_RGB(255, 0, 0), 2);
    line(image, origin, yAxis, CV_RGB(0, 255, 0), 2);
    line(image, origin, zAxis, CV_RGB(0, 0, 255), 2);

    putText(image, "X", xAxis, fontFace, fontScale, Scalar::all(255), 1, 8);
    putText(image, "Y", yAxis, fontFace, fontScale, Scalar::all(255), 1, 8);
    putText(image, "Z", zAxis, fontFace, fontScale, Scalar::all(255), 1, 8);
  }

  imshow(name, image);

  return 0;
}
