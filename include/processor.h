#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  Processor();
  float Utilization();

 private:
   long prev_total_;
   long prev_idle_;
};

#endif
