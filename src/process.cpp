#include "process.h"
#include "linux_parser.h"
#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::stol;
using std::string;
using std::vector;

Process::Process(int pid) { _pid = pid; }

int Process::Pid() const { return _pid; }
long int Process::UpTime() { return LinuxParser::UpTime(Pid()); }
float Process::CpuUtilization() const {
  return LinuxParser::ActiveJiffies(Pid()) / float(LinuxParser::UpTime(Pid()));
}
string Process::Command() { return LinuxParser::Command(Pid()); }
string Process::User() { return LinuxParser::User(Pid()); }
string Process::Ram() const { return LinuxParser::Ram(Pid()); }

bool Process::operator<(Process const& p) const {
  return stol(Ram()) < stol(p.Ram());
}