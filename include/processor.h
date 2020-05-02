#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  long user {0};
  long nice {0};
  long system {0};
  long idle {0};
  long iowait {0};
  long irq {0};
  long softirq {0};
  long steal {0};
  long guest {0};
  long guest_nice {0};
};

#endif