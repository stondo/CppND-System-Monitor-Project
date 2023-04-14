#include "system.h"
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

using std::set;
using std::size_t;
using std::string;
using std::sort;
using std::vector;

Processor& System::Cpu() { return _cpu; }

vector<Process>& System::Processes() {
  vector<int> pids = LinuxParser::Pids();

  _processes.clear();
  for (const int& pid : pids) {
    Process process(pid);
    _processes.push_back(process);
  }
  
  sort(_processes.rbegin(), _processes.rend());
  return _processes;
}

std::string System::Kernel() {
  return LinuxParser::Kernel();
}

float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

std::string System::OperatingSystem() {
  return LinuxParser::OperatingSystem();
  ;
}

int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
  ;
}

int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

float System::UpTime() {
  return LinuxParser::UpTime();
}