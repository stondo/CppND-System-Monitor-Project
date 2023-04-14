#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();
  void UpdateValues(long idle, long active, long total);

 private:
  long _idle;
  long _active;
  long _total;
};

#endif