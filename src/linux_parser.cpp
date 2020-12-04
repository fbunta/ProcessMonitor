#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel >> version;
  }
  return version;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string line;
  string key;
  string value;
  float memTotal = 0.0;
  float memFree = 0.0;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "MemTotal:") {
          memTotal = std::stof(value);
        } else if (key == "MemFree:") {
          memFree = std::stof(value);
        }
      }
      if (memTotal != 0.0 && memFree != 0.0) {
        break;
      }
    }
  }
  return (memTotal - memFree)/ memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string line;
  long upTime;
  std::ifstream filestream(kProcDirectory + kUptimeFilename);
  std::getline(filestream, line);
  upTime = std::stol(line);
  return upTime;
}

// // TODO: Read and return the number of jiffies for the system
// long LinuxParser::Jiffies() { return 0; }

// // TODO: Read and return the number of active jiffies for a PID
// // REMOVE: [[maybe_unused]] once you define the function
// long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// // TODO: Read and return the number of active jiffies for the system
// long LinuxParser::ActiveJiffies() { return 0; }

// // TODO: Read and return the number of idle jiffies for the system
// long LinuxParser::IdleJiffies() { return 0; }

// // TODO: Read and return CPU utilization
// vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses () {
  string line;
  int processCount{0};
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if (key == "processes") {
        processCount = std::stoi(value);
      }
    }
  }
  return processCount;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  int processCount;
  string key, value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  while (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    while (linestream >> key >> value) {
      if (key == "procs_running") {
        processCount = std::stoi(value);
      }
    }
  }
  return processCount;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::getline(filestream, line);
  return line;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize:") {
          return value;
        } else {
          break;
        }
      }
    }
  }
  return "N/A";
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string line;
  string key;
  string value;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "Uid:") {
          return value;
        } else {
          break;
        }
      }
    }
  }
  return "N/A";
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int uid) {
  string line;
  string user;
  string something;
  string string_uid;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> something >> string_uid) {
        if (string_uid == std::to_string(uid)) {
          return user;
        } else {
          break;
        }
      }
    }
  }
  return "N/A";
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string uptime;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string nth;
      for (int i = 1; i < 23; ++i) {
        linestream >> nth;
        if (i == 22) {
          uptime = nth; 
        }
      }
    }
  }
  float Hertz;
  long output;
  Hertz = sysconf(_SC_CLK_TCK);
  output = (std::stof(uptime) / Hertz);
  return output;
}

// Unsure why this wasnt a TODO
float LinuxParser::Utilization(int pid) {
  // this is the average for the process
  string utime, stime, cutime, cstime, starttime, uptime;
  string line;
  std::ifstream filestream(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      string nth;
      for (int i = 1; i < 23; ++i) {
        linestream >> nth;
        if (i == 14) {
          utime = nth; 
        } else if (i == 15) {
          stime = nth; 
        } else if (i == 16) {
          cutime = nth; 
        } else if (i == 17) {
          cstime = nth; 
        } else if (i == 22) {
          starttime = nth; 
        } else if (i == 1) {
          uptime = nth; 
        }
      }
    }
  }
  // float total_time, seconds, Hertz, cpu_usage;
  // Hertz = sysconf(_SC_CLK_TCK);
  // total_time = std::stof(utime) + std::stof(stime) + std::stof(cstime) + std::stof(cutime);
  // seconds = std::stof(uptime) - (std::stof(starttime) / Hertz);
  // cpu_usage = 100.0 * ((total_time / Hertz) / seconds);
  // return cpu_usage;
  long total_time, seconds, Hertz;
  Hertz = sysconf(_SC_CLK_TCK);
  total_time = std::stol(utime) + std::stol(stime) + std::stol(cstime) + std::stol(cutime);
  seconds = total_time / Hertz;
  float cpu_usage = float(total_time) / float(UpTime(pid));
  return cpu_usage;
}