#include "common.h"

void featureTracking(Mat img_1, Mat img_2, vector<Point2f>& points1, vector<Point2f>& points2, vector<uchar>& status);
void featureDetection(Mat img_1, vector<Point2f>& points1);