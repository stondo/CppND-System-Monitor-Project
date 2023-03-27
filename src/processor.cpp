#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  long total_old, total_new, idle_old, idle_new, active;
  total_new = LinuxParser::Jiffies();
  active = LinuxParser::ActiveJiffies();
  idle_new = LinuxParser::IdleJiffies();

  total_old = Processor::total_;
  idle_old = Processor::idle_;

  UpdateValues(idle_new, active, total_new);

  float total_delta = float(total_new - total_old);
  float idle_detla = float(idle_new - idle_old);

  return (total_delta - idle_detla) / total_delta;
}

void Processor::UpdateValues(long idle, long active, long total) {
  idle_ = idle;
  active_ = active;
  total_ = total;
}