#include "mono-vo.h"

int main(int argc, char** argv)	{

  Camera camera;
  Plotter plotter;
  Preview preview("Road facing camera");
  Logger logger("results.txt");

  //TODO: add a fucntion to load these values directly from KITTI's calib files
  // WARNING: different sequences in the KITTI VO dataset have different intrinsic/extrinsic parameters
  camera.init();

  Odometer odometer(camera.getFocal(), camera.getPrinciplePoint());

  clock_t begin = clock();

  int numFrame;
  Mat image;
  Mat colorImage;

  while((numFrame = camera.capture(image)) >= 0) {
    double x, y, z;

    try {
      odometer.estimate(image, camera.getScale(), x, y, z);
    } catch(Exception e) {
      cerr << "Exception occurred." << e.what() << endl;
      odometer = Odometer(camera.getFocal(), camera.getPrinciplePoint());
      continue;
    }

    Pose truth;
    // Pose::get(truth, "../data/01/poses.txt", numFrame);

    logger.log(x, y, z);
    plotter.plot(x, y, z, truth);

    cvtColor(image, colorImage, COLOR_GRAY2RGB);
    preview.redraw(colorImage, odometer.getLastFeatures(), odometer.getCurrFeatures(), odometer.getR());

    waitKey(100);
  }

  clock_t end = clock();
  double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
  cout << "Total time taken: " << elapsed_secs << "s" << endl;

  waitKey(10000);

  return 0;
}
