#include "processor.h"
#include "linux_parser.h"
#include <string>

using namespace std;
using namespace LinuxParser;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  vector<long> cpu;
  vector<string> stats;
  stats = LinuxParser::CpuUtilization();
  for (string x : stats) {
    cpu.push_back(stol(x));
  }
  long PrevIdle = this->idle + this->iowait;
  long Idle = cpu[CPUStates::kIdle_] + cpu[CPUStates::kIOwait_];
  long PrevNonIdle = this->user + this->nice + this->system + this->irq + this->softirq + this->steal;
  long NonIdle = cpu[CPUStates::kUser_] + cpu[CPUStates::kNice_] + cpu[CPUStates::kSystem_] + cpu[CPUStates::kIRQ_] + cpu[CPUStates::kSoftIRQ_] + cpu[CPUStates::kSteal_];
  long PrevTotal = PrevIdle + PrevNonIdle;
  long Total = Idle + NonIdle;
  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;
  float CPU_Percentage = (totald - idled) / totald;
  this->user = cpu[CPUStates::kUser_];
  this->nice = cpu[CPUStates::kNice_];
  this->system = cpu[CPUStates::kSystem_];
  this->idle = cpu[CPUStates::kIdle_];
  this->iowait = cpu[CPUStates::kIOwait_];
  this->irq = cpu[CPUStates::kIRQ_];
  this->softirq = cpu[CPUStates::kSoftIRQ_];
  this->steal = cpu[CPUStates::kSteal_];
  this->guest = cpu[CPUStates::kGuest_];
  this->guest_nice = cpu[CPUStates::kGuestNice_];
  return CPU_Percentage;
}

