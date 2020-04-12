#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "cpu_time.h"

#include <string>

class Processor {
 public:
  Processor(std::string name, CpuTime cpu_time);
  std::string Name();
  float Utilization();
  void UpdateUtilization(CpuTime cpu_time);

 private:
   std::string name_;
   long prev_total_;
   long prev_idle_;
   long totald_;
   long idled_;
};

#endif
