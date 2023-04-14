#ifndef PROCESS_H
#define PROCESS_H

#include <string>

class Process {
 public:
  int Pid() const; 
  long UpTime();
  float CpuUtilization() const; 
  
  std::string Command();    
  std::string Ram() const;     
  std::string User();    
  
  Process(int pid);
  bool operator<(Process const& a) const; 

 private:
  int _pid;
};

#endif