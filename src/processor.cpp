#include "processor.h"
#include "linux_parser.h"

float Processor::Utilization() {
  long jiffies = LinuxParser::Jiffies();
  long active = LinuxParser::ActiveJiffies();
  long idle_new = LinuxParser::IdleJiffies();

  long total_old = Processor::_total;
  long idle_old = Processor::_idle;

  UpdateValues(idle_new, active, jiffies);

  float total_delta = jiffies - total_old;
  float idle_detla = idle_new - idle_old;

  return (total_delta - idle_detla) / total_delta;
}

void Processor::UpdateValues(long idle, long active, long total) {
  _idle = idle;
  _active = active;
  _total = total;
}