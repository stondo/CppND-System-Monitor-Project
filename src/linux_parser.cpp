#include "linux_parser.h"

#include <dirent.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

using std::stof;
using std::stoi;
using std::stol;
using std::string;
using std::to_string;
using std::tuple;
using std::unordered_map;
using std::vector;

const int ZERO = 0;
const int ONE_KILOBYTE = 1024;
const string ZERO_STR = "0";
const string EMPTY_STRING = "";
auto SC_CLK_TK = sysconf(_SC_CLK_TCK);

string LinuxParser::OperatingSystem() {
  string l, k, val;
  std::ifstream filestream(kOSPath);

  string os;
  string name;
  string version;
  if (filestream.is_open()) {
    string delimiter = "=";
    while (std::getline(filestream, l)) {
      string token = l.substr(0, l.find(delimiter));
      if (token == "NAME") {
        name = l.substr(l.find(delimiter));
      }

      if (token == "VERSION") version = l.substr(l.find(delimiter));
    }
  }

  std::replace(name.begin(), name.end(), '"', ' ');
  std::replace(name.begin(), name.end(), '=', ' ');
  std::replace(version.begin(), version.end(), '"', ' ');
  std::replace(version.begin(), version.end(), '=', ' ');

  name.erase(name.begin(),
             std::find_if(name.begin(), name.end(),
                          std::not1(std::ptr_fun<int, int>(std::isspace))));
  name.erase(std::find_if(name.rbegin(), name.rend(),
                          std::not1(std::ptr_fun<int, int>(std::isspace)))
                 .base(),
             name.end());

  version.erase(version.begin(),
                std::find_if(version.begin(), version.end(),
                             std::not1(std::ptr_fun<int, int>(std::isspace))));
  version.erase(std::find_if(version.rbegin(), version.rend(),
                             std::not1(std::ptr_fun<int, int>(std::isspace)))
                    .base(),
                version.end());
  os = name + " " + version;
  return os;
}

string LinuxParser::Kernel() {
  string os, version, kernel, l;
  auto path = kProcDirectory + kVersionFilename;
  std::ifstream strm(path);

  if (strm.is_open()) {
    std::getline(strm, l);
    std::istringstream lstrm(l);
    lstrm >> os >> version >> kernel;
  }

  return kernel;
}

vector<int> LinuxParser::Pids() {
  DIR* dir = opendir(kProcDirectory.c_str());
  vector<int> pids;
  struct dirent* file;

  while ((file = readdir(dir)) != nullptr) {
    if (file->d_type == DT_DIR) {
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(dir);

  return pids;
}

float LinuxParser::MemoryUtilization() {
  string l, k, val, memory_total, memory_free;
  auto path = kProcDirectory + kMeminfoFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::istringstream lstrm(l);
      while (lstrm >> k >> val) {
        if (k == "MemTotal:") {
          memory_total = val;
        } else if (k == "MemFree:") {
          memory_free = val;
        }
      }
    }
  }

  return (stof(memory_total) - stof(memory_free)) / stof(memory_total);
}

float LinuxParser::UpTime() {
  string uptime, l;
  auto path = kProcDirectory + kUptimeFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    std::getline(strm, l);
    std::istringstream lstrm(l);
    lstrm >> uptime;
  }

  float res = stof(uptime);
  return res;
}

long LinuxParser::Jiffies() {
  return LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies();
}

long LinuxParser::ActiveJiffies(int pid) {
  string l, val;
  vector<string> vals;
  auto path = kProcDirectory + std::to_string(pid) + kStatFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    std::getline(strm, l);
    std::istringstream lstrm(l);
    while (lstrm >> val) {
      vals.push_back(val);
    }
  }

  long total_time = ZERO;
  unordered_map<string, int> times_map = {
      {"u_time", 13}, {"s_time", 14}, {"cu_time", 15}, {"cs_time", 16}};

  for (const auto& [k, t] : times_map)
    if (std::all_of(vals[t].begin(), vals[t].end(), isdigit))
      total_time += stol(vals[t]);

  return total_time / SC_CLK_TK;
}

long LinuxParser::ActiveJiffies() {
  auto jiffies = CpuUtilization();
  auto cpu_states = {CPUStates::kUser_, CPUStates::kSystem_,
                     CPUStates::kNice_, CPUStates::kSteal_,
                     CPUStates::kIRQ_,  CPUStates::kSoftIRQ_};

  long sum = ZERO;
  for (const auto& state : cpu_states) sum += stol(jiffies[state]);

  return sum;
}

long LinuxParser::IdleJiffies() {
  auto jiffies = CpuUtilization();
  return stol(jiffies[CPUStates::kIOwait_]) + stol(jiffies[CPUStates::kIdle_]);
}

vector<string> LinuxParser::CpuUtilization() {
  string l, k, val;
  vector<string> vals;
  auto path = kProcDirectory + kStatFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    std::getline(strm, l);
    std::istringstream lstrm(l);
    lstrm >> k;
    while (lstrm >> val) {
      vals.push_back(val);
    };
  }

  return vals;
}

int LinuxParser::TotalProcesses() {
  string l, k, val;
  auto path = kProcDirectory + kStatFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::istringstream lstrm(l);
      while (lstrm >> k >> val) {
        if (k == "processes") {
          return stoi(val);
        }
      }
    }
  }

  return ZERO;
}

int LinuxParser::RunningProcesses() {
  string l, k, val;
  auto path = kProcDirectory + kStatFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::istringstream lstrm(l);
      while (lstrm >> k >> val) {
        if (k == "procs_running") {
          return stoi(val);
        }
      }
    }
  }

  return ZERO;
}

string LinuxParser::Command(int pid) {
  string cmd;
  auto path = kProcDirectory + to_string(pid) + kCmdlineFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    std::getline(strm, cmd);
  }

  return cmd;
}

string LinuxParser::Ram(int pid) {
  string l, k, val, mem_used = ZERO_STR;
  auto path = kProcDirectory + to_string(pid) + kStatusFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::istringstream lstrm(l);
      while (lstrm >> k) {
        if (k == "VmSize:") {
          lstrm >> val;
          return to_string(stol(val) / ONE_KILOBYTE);
        }
      }
    }
  }

  return mem_used;
}

string LinuxParser::Uid(int pid) {
  string l, k, val;
  auto path = kProcDirectory + to_string(pid) + kStatusFilename;
  std::ifstream strm(path);

  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::istringstream lstrm(l);
      while (lstrm >> k >> val) {
        if (k == "Uid:") {
          return val;
        }
      }
    }
  }
  return EMPTY_STRING;
}

string LinuxParser::User(int pid) {
  string l, name, y, uid;

  std::ifstream strm(kPasswordPath);
  if (strm.is_open()) {
    while (std::getline(strm, l)) {
      std::replace(l.begin(), l.end(), ':', ' ');
      std::istringstream lstrm(l);
      while (lstrm >> name >> y >> uid) {
        if (uid == LinuxParser::Uid(pid)) {
          return name;
        }
      }
    }
  }
  return EMPTY_STRING;
}

long LinuxParser::UpTime(int pid) {
  string l, val;
  vector<string> vals;
  auto path = kProcDirectory + to_string(pid) + kStatFilename;

  std::ifstream strm(path);
  if (strm.is_open()) {
    std::getline(strm, l);
    std::istringstream lstrm(l);

    while (lstrm >> val) {
      vals.push_back(val);
    };
  }

  return LinuxParser::UpTime() - (stol(vals[21]) / SC_CLK_TK);
}
