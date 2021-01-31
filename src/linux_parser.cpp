#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "linux_parser.h"

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
  return "NOT FOUND";
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
    string memTotalString = GetValue("MemTotal",kProcDirectory + kMeminfoFilename, ':', true);
    float memTotal = (float) StringToLong(memTotalString);
    string memFreeString = GetValue("MemFree",kProcDirectory + kMeminfoFilename, ':', true);
    float memFree = (float) StringToLong(memFreeString);
    float memUtil = (memTotal-memFree)/memTotal;
    return memUtil;
}

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
    string upTimeString = SplitFile(kProcDirectory + kUptimeFilename, 0);
    long upTime = StringToLong(upTimeString);
    return upTime;
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
    long activeJiffies = ActiveJiffies();
    long idleJiffies = IdleJiffies();
    return activeJiffies + idleJiffies; }

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) { 
    string statPath = LinuxParser::kProcDirectory + "/" + to_string(pid) + LinuxParser::kStatFilename;
    long utime = StringToLong(LinuxParser::SplitFile(statPath, 13));
    long stime = StringToLong(LinuxParser::SplitFile(statPath, 14));
    long cutime = StringToLong(LinuxParser::SplitFile(statPath, 15));
    long cstime = StringToLong(LinuxParser::SplitFile(statPath, 16));
    long totalTime = utime + stime + cutime + cstime;
    return totalTime; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
    vector<int> indices = {1, 2, 3, 6, 7, 8};
    return RetrieveCpuSum(indices); 
}

long LinuxParser::RetrieveCpuSum(vector<int> indices){
    long totalJiffies = 0;
    for(int index : indices){
        string JiffiesString = SplitFile(kProcDirectory + kStatFilename, index);
        totalJiffies += StringToLong(JiffiesString);
    }
    return totalJiffies; 
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
    vector<int> indices = {4, 5};
    return RetrieveCpuSum(indices); 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
    string totalProcessesString = GetValue("processes", kProcDirectory + kStatFilename, ' ', false);
    int totalProcesses = std::stoi(totalProcessesString);
    return totalProcesses;
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
    string runningProcessesString = GetValue("procs_running", kProcDirectory + kStatFilename, ' ', false);
    int runningProcesses = std::stoi(runningProcessesString);
    return runningProcesses;
}

// TODO: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
    string commandPath = kProcDirectory + "/" + to_string(pid) + kCmdlineFilename;
    string command;
    std::ifstream filestream(commandPath);
    if (filestream.is_open()) {
        std::getline(filestream, command);
    }
    return command;
}

// TODO: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
    string statusPath = kProcDirectory + "/" + to_string(pid) + kStatusFilename;
    string ramUsageKb = GetValue("VmSize", statusPath, ':', true);
    long ramUsageMb = StringToLong(ramUsageKb) / 1000;
    string ramUsage = to_string(ramUsageMb);
    return ramUsage;
}

// TODO: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
    string statusPath = kProcDirectory + "/" + to_string(pid) + kStatusFilename;
    string userId = GetValue("Uid", statusPath, ':', true);
    return userId;
}

// TODO: Read and return the user associated with a process
string LinuxParser::User(int pid) { 

    string userId = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    string userName = "";
    string line;
    string curUid;
    string whatever;
    if (filestream.is_open()) {
        while(std::getline(filestream, line)){
            std::replace(line.begin(), line.end(), ' ', '_');
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> userName >> whatever >> curUid;
            if(curUid == userId){
                return userName;
            }
        }
    }
    return "NOTFOUND";
}

// TODO: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 
    string upTimePath = LinuxParser::kProcDirectory + "/" + to_string(pid) + LinuxParser::kStatFilename;
    string upTimeStr = LinuxParser::SplitFile(upTimePath, 21);
    long upTimeTicks = StringToLong(upTimeStr);
    long clockTicks = sysconf(_SC_CLK_TCK);
    long upTimeOffsetSeconds = upTimeTicks / clockTicks;
    long upTimeSeconds = UpTime() - upTimeOffsetSeconds;
    return upTimeSeconds;
}

long LinuxParser::StringToLong(string text){
    long number;
    try {
        number = std::stol(text);
    } catch (...) {
        number = 0;
    }
    return number;
}
