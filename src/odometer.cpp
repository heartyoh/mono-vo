#include "odometer.hpp"

#define MIN_NUM_FEAT 2000

Odometer::Odometer(double focal, Point2d pp) {
  this->focal = focal;
  this->pp = pp;
}

vector<Point2f>& Odometer::getLastFeatures() {
  return lastFeatures;
}

vector<Point2f>& Odometer::getCurrFeatures() {
  return currFeatures;
}

Mat& Odometer::getR() {
  return R;
}

void Odometer::featureTracking(Mat prevImage, Mat currImage, vector<Point2f>& prevFeatures, vector<Point2f>& currFeatures, vector<uchar>& status) { 

  // 트래킹에 실패한 포인트들은 버린다.
  vector<float> err;          
  Size winSize = Size(21,21);                                               
  TermCriteria termcrit = TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01);

  calcOpticalFlowPyrLK(prevImage, currImage, prevFeatures, currFeatures, status, err, winSize, 3, termcrit, 0, 0.001);

  // KLT 트래킹에 실패하거나 프레임 바깥으로 벗어난 포인트들을 버린다.
  int indexCorrection = 0;
  for(int i = 0;i < status.size();i++) {  
    Point2f pt = currFeatures.at(i - indexCorrection);

    if((status.at(i) == 0)||(pt.x < 0)||(pt.y < 0)) {

      if((pt.x < 0) || (pt.y < 0))
        status.at(i) = 0;

      prevFeatures.erase (prevFeatures.begin() + (i - indexCorrection));
      currFeatures.erase (currFeatures.begin() + (i - indexCorrection));

      indexCorrection++;
    }
  }
}

void Odometer::featureDetection(Mat image, vector<Point2f>& features)  {
  // uses FAST as of now, modify parameters as necessary
  vector<KeyPoint> keypoints;
  int fast_threshold = 20;
  bool nonmaxSuppression = true;
 
  FAST(image, keypoints, fast_threshold, nonmaxSuppression);
 
  KeyPoint::convert(keypoints, features, vector<int>());
}

int Odometer::estimate(Mat currImage, double scale, double& x, double& y, double& z) {

  if(prevImage.empty()) {
    prevImage = currImage;
    featureDetection(prevImage, prevFeatures);

    return 0;
  }

  featureTracking(prevImage, currImage, prevFeatures, currFeatures, status);

  E = findEssentialMat(currFeatures, prevFeatures, focal, pp, RANSAC, 0.999, 1.0, mask);
  recoverPose(E, currFeatures, prevFeatures, R, t, focal, pp, mask);

  Mat prevPts(2, prevFeatures.size(), CV_64F), currPts(2, currFeatures.size(), CV_64F);

  for(int i = 0;i < prevFeatures.size();i++) {
    //this (x,y) combination makes sense as observed from the source code of triangulatePoints on GitHub
    prevPts.at<double>(0, i) = prevFeatures.at(i).x;
    prevPts.at<double>(1, i) = prevFeatures.at(i).y;

    currPts.at<double>(0, i) = currFeatures.at(i).x;
    currPts.at<double>(1, i) = currFeatures.at(i).y;
  }

  if(R_f.empty()) {
    R_f = R.clone();
    t_f = t.clone();
  }

  if(scale > 0.1 
    && (t.at<double>(2) > t.at<double>(0)) 
    && (t.at<double>(2) > t.at<double>(1))
  ) {

    t_f = t_f + scale * (R_f * t);
    R_f = R * R_f;
  } else {
    cout << "scale below 0.1, or incorrect translation" << endl;
  }

  // 피쳐의 갯수가 작아지면, 다시 검출함.
  if(prevFeatures.size() < MIN_NUM_FEAT) {
    featureDetection(prevImage, prevFeatures);
    featureTracking(prevImage, currImage, prevFeatures, currFeatures, status);
  }

  x = t_f.at<double>(0);
  y = t_f.at<double>(1);
  z = t_f.at<double>(2);

  prevImage = currImage;
  lastFeatures = prevFeatures;
  prevFeatures = currFeatures;

  return 0;
}