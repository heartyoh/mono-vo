#include "mono-vo.h"

int main(int argc, char** argv)	{

  Camera camera;
  Plotter plotter;
  Playback playback("Road facing camera");
  Logger logger("results.txt");

  //TODO: add a fucntion to load these values directly from KITTI's calib files
  // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
  camera.init();

  Odometer odometer(camera.getFocal(), camera.getPrinciplePoint());

  clock_t begin = clock();

  int numFrame;
  Mat image;

  while((numFrame = camera.capture(image)) >= 0) {
    double x, y, z;

    odometer.estimate(image, camera.getScale(), x, y, z);    

    Pose truth;
    Pose::get(truth, "../data/01/poses.txt", numFrame);

    logger.log(x, y, z);
    plotter.plot(x, y, z, truth);
    playback.redraw(image);

    waitKey(1);
  }

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Total time taken: " << elapsed_secs << "s" << endl;

  waitKey(10000);

  return 0;
}