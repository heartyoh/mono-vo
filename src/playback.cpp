#include "playback.hpp"

Playback::Playback(const char* name) {
  bzero(this->name, 100);
  strncpy(this->name, name, strlen(name));
  namedWindow(this->name, WINDOW_AUTOSIZE);
}

int Playback::redraw(Mat image) {
  imshow(this->name, image);
  return 0;
}