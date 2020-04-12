#include "processor.h"
#include "linux_parser.h"
#include "cpu_time.h"


float CalculateRate(long total_time, long idle_time) {
  double active_time = total_time - idle_time;
  return active_time / total_time;
}


Processor::Processor() {
  CpuTime cpu_time = LinuxParser::CpuUtilization();
  prev_total_ = cpu_time.total_time;
  prev_idle_ = cpu_time.idle_time;
}

float Processor::Utilization() {
  CpuTime cpu_time = LinuxParser::CpuUtilization();
  if (cpu_time.total_time == prev_total_) {
    return CalculateRate(prev_total_, prev_idle_);
  }
  long totald = cpu_time.total_time - prev_total_;
  long idled = cpu_time.idle_time - prev_idle_;
  prev_total_ = cpu_time.total_time;
  prev_idle_ = cpu_time.idle_time;
  return CalculateRate(totald, idled);
}
