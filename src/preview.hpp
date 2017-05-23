#include "common.h"

class Preview {

  char name[100];
  Mat AXIS;
  int fontFace;
  double fontScale;

public:
  Preview(const char* name);

  int redraw(Mat image, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures, Mat& R);
};
