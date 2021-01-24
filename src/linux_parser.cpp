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
  string value = LinuxParser::GetValue("PRETTY_NAME", kOSPath, '=', true);
  return value;
}

// Helper Function to get value for given key from key value file
string LinuxParser::GetValue(string name, string path, char divider, bool replacements){
  string line;
  string key;
  string value;
  std::ifstream filestream(path);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(replacements) {
        std::replace(line.begin(), line.end(), ' ', '_');
        std::replace(line.begin(), line.end(), divider, ' ');
        std::replace(line.begin(), line.end(), '"', ' ');
      }
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == name) {
          if(replacements) {
            std::replace(value.begin(), value.end(), '_', ' ');
          }
          return value;
        }
      }
    }
  }
}


// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
    string kernel = LinuxParser::SplitFile(kProcDirectory + kVersionFilename, 2);
    return kernel;
}

string LinuxParser::SplitFile(string path, int index){
  string selectedString;
  string line;
  int i = 0;
  std::ifstream stream(path);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while(i <= index){
        linestream >> selectedString;
        i++;
    }
  }
  return selectedString;
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
    string memTotalString = LinuxParser::GetValue("MemTotal",kProcDirectory + kMeminfoFilename, ':', true);
    float memTotal = std::stof(memTotalString);
    string memFreeString = LinuxParser::GetValue("MemFree",kProcDirectory + kMeminfoFilename, ':', true);
    float memFree = std::stof(memFreeString);
    float memUtil = (memTotal-memFree)/memTotal;
    return memUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string upTimeString = SplitFile(kProcDirectory + kUptimeFilename, 0);
    long upTime = std::stol(upTimeString);
    return upTime;
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
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string totalProcessesString = LinuxParser::GetValue("processes", kProcDirectory + kStatFilename, ' ', false);
    int totalProcesses = std::stoi(totalProcessesString);
    return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string runningProcessesString = LinuxParser::GetValue("procs_running", kProcDirectory + kStatFilename, ' ', false);
    int runningProcesses = std::stoi(runningProcessesString);
    return runningProcesses;
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }
