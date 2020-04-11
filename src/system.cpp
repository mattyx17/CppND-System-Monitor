#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

System::System() {
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
  vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    processes_.push_back(Process(pid));
  }
}

Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() { return processes_; }


string System::Kernel() { return kernel_; }


float System::MemoryUtilization() {
  return LinuxParser::MemoryUtilization();
}


string System::OperatingSystem() { return os_; }


int System::RunningProcesses() {
  return LinuxParser::RunningProcesses();
}


int System::TotalProcesses() {
  return LinuxParser::TotalProcesses();
}


long int System::UpTime() {
  return LinuxParser::UpTime();
}
