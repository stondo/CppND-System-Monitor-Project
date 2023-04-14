#include "format.h"

#include <string>
#include <iostream>
#include <iomanip>

using std::string;
using std::to_string;
using std::stringstream;

static const size_t n_zero = 2;

string Format::ElapsedTime(long seconds) {
  int hrs, mins, secs;
  stringstream formatted_time;
  hrs = (seconds / 3600) % 100;
  mins = (seconds / 60) % 60;
  secs = seconds % 60;

  string hrs_with_zeros = string(n_zero - std::min(n_zero, to_string(hrs).length()), '0') + to_string(hrs);
  string mins_with_zeros = string(n_zero - std::min(n_zero, to_string(mins).length()), '0') + to_string(mins);
  string secs_with_zeros = string(n_zero - std::min(n_zero, to_string(secs).length()), '0') + to_string(secs);
  
  formatted_time << hrs_with_zeros << ":" << mins_with_zeros << ":" << secs_with_zeros;
  return formatted_time.str();
}