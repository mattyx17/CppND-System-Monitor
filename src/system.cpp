#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

vector<Process> CleanProcesses(vector<Process> processes) {
  vector<int> pids = LinuxParser::Pids();
  std::sort(pids.begin(), pids.end());
  vector<Process> clean_processes = {};
  vector<int> old_ids = {};
  for (Process p : processes) {
    if (std::binary_search(pids.begin(), pids.end(), p.Pid())) {
      clean_processes.push_back(p);
      old_ids.push_back(p.Pid());
    }
  }
  std::sort(old_ids.begin(), old_ids.end());
  for (int pid : pids) {
    if (!(std::binary_search(old_ids.begin(), old_ids.end(), pid))) {
      clean_processes.push_back(Process(pid));
    }
  }
  return clean_processes;
}

System::System() {
  os_ = LinuxParser::OperatingSystem();
  kernel_ = LinuxParser::Kernel();
  vector<int> pids = LinuxParser::Pids();
  for (int pid : pids) {
    processes_.push_back(Process(pid));
  }
}

Processor& System::Cpu() { return cpu_; }


vector<Process>& System::Processes() {
  processes_ = CleanProcesses(processes_);
  std::sort(processes_.rbegin(), processes_.rend());
  return processes_;
}


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
