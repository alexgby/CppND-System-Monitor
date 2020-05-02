#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>

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
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
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
  float memTotal, memFree;
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = stof(value);
      } else if (key == "MemFree:") {
        memFree = stof(value);
      }
    }
  }
  return (memTotal - memFree) / memTotal;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() {
  string system, idle;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> system >> idle;
  }
  return stol(system);
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line;
  vector<string> tokens;
  string token;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream  tokenStream(line);
    while (std::getline(tokenStream, token, ' ')) {
      if (token != "cpu" && !token.empty()) {
        tokens.push_back(token);
      }
    }
  }
  return tokens;
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  int totalProcesses;
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "processes") {
        totalProcesses = stoi(value);
      }
    }
  }
  return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  int runningProcesses;
  string key, value;
  string line;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "procs_running") {
        runningProcesses = stoi(value);
      }
    }
  }
  return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  const string kPidDirectory {to_string(pid)};
  string line;
  string result;
  std::ifstream stream(kProcDirectory + kPidDirectory + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    result = line;
  }
  return result;
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) {
  const string kPidDirectory {to_string(pid)};
  string result;
  string key, value;
  string line;
  int vmsize;
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "VmSize:") {
        vmsize = stoi(value) / 1024;
        return to_string(vmsize);
      }
    }
  }
  return result;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid) {
  const string kPidDirectory {to_string(pid)};
  string result;
  string key, value;
  string line;
  vector<string> tokens;
  string token;
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "Uid:") {
        result = value;
      }
    }
  }
  if (result.size() == 0) {
    throw "Can't get UID";
  }
  return result;
}

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  string line;
  string key;
  string value;
  string null;
  string uid = LinuxParser::Uid(pid);
  std::ifstream filestream(kPasswordPath);
  vector<string> tokens;
  string token;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      tokens.clear();
      while (linestream >> token) {
        if (!token.empty()) {
          std::replace(token.begin(), token.end(), '_', ' ');
          tokens.push_back(token);
        }
      }
      if (tokens[2] == uid) {
        return tokens[0] + "    ";
      }
    }
  }
  return key;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid) {
  const string kPidDirectory {to_string(pid)};
  string system, idle;
  string line;
  vector<string> tokens;
  string token;
  long starttime;
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream  tokenStream(line);
    while (std::getline(tokenStream, token, ' ')) {
      tokens.push_back(token);
    }
  } else {
    throw "Open file failed!";
  }
  starttime = stol(tokens[21]);
  return starttime / sysconf(_SC_CLK_TCK);
}

float LinuxParser::CpuUtilization(int pid) {
  const string kPidDirectory {to_string(pid)};
  string line;
  vector<string> tokens;
  string token;
  std::ifstream stream(kProcDirectory + kPidDirectory + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream  tokenStream(line);
    while (std::getline(tokenStream, token, ' ')) {
      tokens.push_back(token);
    }
  }
  float uptime;
  string system, idel;
  std::ifstream ustream(kProcDirectory + kUptimeFilename);
  if (ustream.is_open()) {
    std::getline(ustream, line);
    std::istringstream linestream(line);
    linestream >> system >> idel;
    uptime = stof(system);
  }

  long utime = stol(tokens[13]);
  long stime = stol(tokens[14]);
  long cutime = stol(tokens[15]);
  long cstime = stol(tokens[16]);
  long starttime = stol(tokens[21]);
  long total_time = utime + stime;
  total_time += (cutime + cstime);
  float second = uptime - ((float) starttime / sysconf(_SC_CLK_TCK));
  float cpu_usage = 100 * ((total_time / sysconf(_SC_CLK_TCK)) / second);
  return cpu_usage;
}
