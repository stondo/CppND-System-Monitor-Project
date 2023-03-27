#include "process.h"
#include "linux_parser.h"

#include <unistd.h>

#include <cctype>
#include <sstream>
#include <string>
#include <vector>

using std::string;
using std::to_string;
using std::vector;
using std::stol;

Process::Process(int pid) {
  pid_ = pid;
}

// TODO: Return this process's ID
int Process::Pid() const {
  return pid_;
}

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() const {
  float actj_time = LinuxParser::ActiveJiffies(Pid());
  float up_time = float(LinuxParser::UpTime(Pid()));
  return actj_time/up_time;
}

// TODO: Return the command that generated this process
string Process::Command() {
    return LinuxParser::Command(Pid());
}

// TODO: Return this process's memory utilization
string Process::Ram() const {
    return LinuxParser::Ram(Pid());
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    return LinuxParser::User(Pid());
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    return LinuxParser::UpTime(Pid());
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return stol(Ram()) < stol(a.Ram());
}