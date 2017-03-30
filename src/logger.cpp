#include "logger.hpp"

Logger::Logger(const char* path) {
  
  this->file.open(path);

  cout << "Logger created.\n";
}

int Logger::log(double dx, double dy, double dz) {
  this->file << dx << " " << dy << " " << dz << endl;

  return 0;
}