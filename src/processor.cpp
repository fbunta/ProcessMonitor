#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <regex>

#include "processor.h"
#include "linux_parser.h"

using std::string;
using std::cout;
using std::ifstream;

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
  string cpu, _user, _nice, _system, _idle, _iowait, _irq, _softirq, _steal;
  long prevuser, prevnice, prevsystem, previdle, previowait, previrq, prevsoftirq, prevsteal;
  long user, nice, system, idle, iowait, irq, softirq, steal;
  string line;
  float CPU_Percentage {0.0};
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  std::getline(filestream, line);
  std::istringstream linestream(line);
  linestream >> cpu >> _user >> _nice >> _system >> _idle >> _iowait >> _irq >> _softirq >> _steal;
  prevuser = std::stol(_user);
  prevnice = std::stol(_nice);
  prevsystem = std::stol(_system);
  previdle = std::stol(_idle);
  previowait = std::stol(_iowait);
  previrq = std::stol(_irq);
  prevsoftirq = std::stol(_softirq);
  prevsteal = std::stol(_steal);

  sleep(5);
  std::getline(filestream, line);
  std::istringstream linestream2(line);
  linestream2 >> cpu >> _user >> _nice >> _system >> _idle >> _iowait >> _irq >> _softirq >> _steal;
  user = std::stol(_user);
  nice = std::stol(_nice);
  system = std::stol(_system);
  idle = std::stol(_idle);
  iowait = std::stol(_iowait);
  irq = std::stol(_irq);
  softirq = std::stol(_softirq);
  steal = std::stol(_steal);

  long PrevIdle = previdle + previowait;
  long Idle = idle + iowait;
  long PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal;
  long NonIdle = user + nice + system + irq + softirq + steal;
  long PrevTotal = PrevIdle + PrevNonIdle;
  long Total = Idle + NonIdle;
  // differentiate: actual value minus the previous one
  long totald = Total - PrevTotal;
  long idled = Idle - PrevIdle;

  CPU_Percentage = (float(totald - idled))/totald;

  return CPU_Percentage;
}