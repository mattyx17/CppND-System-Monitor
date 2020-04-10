#include <iomanip>
#include <sstream>
#include <string>

#include "format.h"

using std::string;

string Format::ElapsedTime(long seconds) {
  int hours = seconds / 3600;
  seconds = seconds % 3600;
  int minutes = seconds / 60;
  seconds = seconds % 60;
  std::ostringstream time;
  time << std::setw(2) << std::setfill('0') << hours << ":";
  time << std::setw(2) << std::setfill('0') << minutes << ":";
  time << std::setw(2) << std::setfill('0') << seconds;
  return time.str();
}
