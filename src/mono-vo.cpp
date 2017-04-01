#include "mono-vo.h"

#define MIN_NUM_FEAT 2000

int main(int argc, char** argv)	{

  Camera camera;
  Plotter plotter;
  Playback playback("Road facing camera");
  Logger logger("results.txt");

  //TODO: add a fucntion to load these values directly from KITTI's calib files
  // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
  camera.init();

  double focal = camera.getFocal();
  double scale = camera.getScale();
  Point2d pp = camera.getPrinciplePoint();

  clock_t begin = clock();

  //vectors to store the coordinates of the feature points
  vector<Point2f> points1, points2; 
  vector<uchar> status;

  Mat img_1, img_2;
  Mat R_f, t_f; //the final rotation and tranlation vectors containing the 

  Mat E, R, t, mask;

  Mat prevImage;
  Mat currImage;
  vector<Point2f> prevFeatures;
  vector<Point2f> currFeatures;

  //read the first two frames from the dataset
  camera.capture(img_1);
  camera.capture(img_2);

  if (!img_1.data || !img_2.data) { 
    cout<< " --(!) Error reading images " << endl; 
    return -1;
  }

  // feature detection, tracking
  featureDetection(img_1, points1);
  featureTracking(img_1, img_2, points1, points2, status);

  //recovering the pose and the essential matrix
  E = findEssentialMat(points2, points1, focal, pp, RANSAC, 0.999, 1.0, mask);
  recoverPose(E, points2, points1, R, t, focal, pp, mask);

  prevImage = img_2;
  prevFeatures = points2;

  R_f = R.clone();
  t_f = t.clone();

  int numFrame;
  while((numFrame = camera.capture(currImage)) >= 0) {

  	featureTracking(prevImage, currImage, prevFeatures, currFeatures, status);

  	E = findEssentialMat(currFeatures, prevFeatures, focal, pp, RANSAC, 0.999, 1.0, mask);
  	recoverPose(E, currFeatures, prevFeatures, R, t, focal, pp, mask);

    Mat prevPts(2, prevFeatures.size(), CV_64F), currPts(2, currFeatures.size(), CV_64F);

    for(int i = 0;i < prevFeatures.size();i++)	{
      //this (x,y) combination makes sense as observed from the source code of triangulatePoints on GitHub
  		prevPts.at<double>(0, i) = prevFeatures.at(i).x;
  		prevPts.at<double>(1, i) = prevFeatures.at(i).y;

  		currPts.at<double>(0, i) = currFeatures.at(i).x;
  		currPts.at<double>(1, i) = currFeatures.at(i).y;
    }

  	scale = camera.getScaleFromDataset();

    if((scale > 0.1)
      &&(t.at<double>(2) > t.at<double>(0)) && (t.at<double>(2) > t.at<double>(1))) {

      t_f = t_f + scale * (R_f * t);
      R_f = R * R_f;
    } else {
      cout << "scale below 0.1, or incorrect translation" << endl;
    }

    // 피쳐의 갯수가 작아지면, 다시 검출함.
 	  if (prevFeatures.size() < MIN_NUM_FEAT)	{
 		  featureDetection(prevImage, prevFeatures);
      featureTracking(prevImage, currImage, prevFeatures, currFeatures, status);
 	  }

    double dx = t_f.at<double>(0);
    double dy = t_f.at<double>(1);
    double dz = t_f.at<double>(2);

    Pose truth;
    Pose::get(truth, "../data/01/poses.txt", numFrame);

    logger.log(dx, dy, dz);
    plotter.plot(dx, dy, dz, truth);
    playback.redraw(currImage);

    prevImage = currImage;
    prevFeatures = currFeatures;

    waitKey(1);
  }

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Total time taken: " << elapsed_secs << "s" << endl;

  waitKey(10000);

  return 0;
}