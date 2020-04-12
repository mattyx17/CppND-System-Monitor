#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;


Process::Process(int pid) {
  pid_ = pid;
  prev_active_time_ = LinuxParser::ActiveTime(pid_);
  double system_uptime = LinuxParser::UpTime();
  double process_uptime = LinuxParser::UpTime(pid_);
  prev_seconds_ = system_uptime - process_uptime;
}


int Process::Pid() { return pid_; }


void Process::SetCpuData() {
  double active_time = LinuxParser::ActiveTime(pid_);
  double system_uptime = LinuxParser::UpTime();
  double process_uptime = LinuxParser::UpTime(pid_);
  double seconds = system_uptime - process_uptime;
  actived_ = active_time - prev_active_time_;
  secondsd_ = seconds - prev_seconds_;
  prev_active_time_ = active_time;
  prev_seconds_ = seconds;
}

float Process::CpuUtilization() const {
  if (secondsd_ == 0 || actived_ == 0) {
    return prev_active_time_ / prev_seconds_;
  }
  return actived_ / secondsd_;
}


string Process::Command() {
  return LinuxParser::Command(pid_);
}


string Process::Ram() {
  return LinuxParser::Ram(pid_);
}


string Process::User() {
  return LinuxParser::User(pid_);
}


long int Process::UpTime() {
  return LinuxParser::UpTime(pid_);
}


bool Process::operator<(Process const& a) const {
  return this->CpuUtilization() < a.CpuUtilization();
}
