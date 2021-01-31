#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
#include "system.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) { 
    this->pid = pid;

}

// TODO: Return this process's ID
int Process::Pid() { return pid; }

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() { 
    float totalTime = LinuxParser::ActiveJiffies(pid);
    long clockTicks = sysconf(_SC_CLK_TCK);
    float totalTimeS = ((float) totalTime / (float) clockTicks);
    long upTime = UpTime();
    float utilization  = totalTimeS / ((float) upTime);

    return utilization;
}

// TODO: Return the command that generated this process
string Process::Command() { 
    string command = LinuxParser::Command(pid);
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { 
    string ramUsage = LinuxParser::Ram(pid);
    return ramUsage;
}

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string userName = LinuxParser::User(pid);
    return userName;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    long upTimeSeconds = LinuxParser::UpTime(pid);
    return upTimeSeconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process& a) { 
    return this->CpuUtilization() > a.CpuUtilization(); 
}
