#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>
#include <map>

#include "process.h"
#include "processor.h"

class System {
 public:
  System();
  std::vector<Processor>& Cpus();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

 private:
  std::vector<Processor> cpus_ = {};
  std::vector<Process> processes_ = {};
  std::string os_;
  std::string kernel_;
};

#endif
