#include "format.h"

#include <string>

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS
// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
  int hrs, mins, secs;
  hrs = (seconds / 3600) % 100;
  mins = (seconds / 60) % 60;
  secs = seconds % 60;

  string secs_str = std::to_string(secs);
  string mins_str = std::to_string(mins);
  string hrs_str = std::to_string(hrs);

  secs_str.insert(0, 2 - secs_str.length(), 0);
  mins_str.insert(0, 2 - mins_str.length(), 0);
  hrs_str.insert(0, 2 - hrs_str.length(), 0);

  return hrs_str + ":" + mins_str + ":" + secs_str;
}