#include "common.h"

class Params {

private:

public:
  static double getScale();

  static double getScaleFromDataset(int frame_id, int sequence_id, double z_cal, double* ox, double* oy, double* oz);
};