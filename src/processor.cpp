#include <string>

#include "processor.h"
#include "linux_parser.h"
#include "cpu_time.h"

using std::string;


float CalculateRate(long total_time, long idle_time) {
  double active_time = total_time - idle_time;
  return active_time / total_time;
}


Processor::Processor(string name, CpuTime cpu_time) : name_(name) {
  prev_total_ = cpu_time.total_time;
  prev_idle_ = cpu_time.idle_time;
}


string Processor::Name() { return name_; }


float Processor::Utilization() {
  if (totald_ == 0) {
    return CalculateRate(prev_total_, prev_idle_);
  }
  return CalculateRate(totald_, idled_);
}

void Processor::UpdateUtilization(CpuTime cpu_time) {
  totald_ = cpu_time.total_time - prev_total_;
  idled_ = cpu_time.idle_time - prev_idle_;
  prev_total_ = cpu_time.total_time;
  prev_idle_ = cpu_time.idle_time;
}
