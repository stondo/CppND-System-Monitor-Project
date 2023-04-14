#ifndef SYSTEM_PARSER_H
#define SYSTEM_PARSER_H

#include <fstream>
#include <regex>
#include <string>

namespace LinuxParser {

  const std::string kProcDirectory{"/proc/"};
  const std::string kCmdlineFilename{"/cmdline"};
  const std::string kCpuinfoFilename{"/cpuinfo"};
  const std::string kStatusFilename{"/status"};
  const std::string kStatFilename{"/stat"};
  const std::string kUptimeFilename{"/uptime"};
  const std::string kMeminfoFilename{"/meminfo"};
  const std::string kVersionFilename{"/version"};
  const std::string kOSPath{"/etc/os-release"};
  const std::string kPasswordPath{"/etc/passwd"};


  int TotalProcesses();
  int RunningProcesses();
  float UpTime();
  float MemoryUtilization();
  std::vector<int> Pids();
  std::string OperatingSystem();
  std::string Kernel();

  enum CPUStates {
    kUser_ = 0,
    kNice_ = 1,
    kSystem_ = 2,
    kIdle_ = 3,
    kIOwait_ = 4,
    kIRQ_ = 5,
    kSoftIRQ_ = 6,
    kSteal_ = 7,
    kGuest_ = 8,
    kGuestNice_ = 9
  };

  std::vector<std::string> CpuUtilization();
  long ActiveJiffies();
  long ActiveJiffies(int pid);
  long IdleJiffies();
  long Jiffies();
  long UpTime(int pid);

  std::string Uid(int pid);
  std::string User(int pid);
  std::string Command(int pid);
  std::string Ram(int pid);
  
}; 

#endif