#include "playback.hpp"

Playback::Playback(const char* name) {
  bzero(this->name, 100);
  strncpy(this->name, name, strlen(name));
  namedWindow(this->name, WINDOW_AUTOSIZE);
}

int Playback::redraw(Mat image, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures) {

  if(prevFeatures.size() == currFeatures.size()) {

    for(int i = 0;i < prevFeatures.size();i++) {
      Point2f prev = prevFeatures.at(i);
      Point2f curr = currFeatures.at(i);
      
      line(image, prev, curr, CV_RGB(0, 255, 0));
    }
  }

  imshow(name, image);
  return 0;
}