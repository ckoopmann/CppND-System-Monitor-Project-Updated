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
float Process::CpuUtilization() { return 0; }

// TODO: Return the command that generated this process
string Process::Command() { 
    string commandPath = LinuxParser::kProcDirectory + "/" + to_string(pid) + LinuxParser::kCmdlineFilename;
    std::ifstream filestream(commandPath);
    if (filestream.is_open()) {
        std::getline(filestream, command);
    }
    return command;
}

// TODO: Return this process's memory utilization
string Process::Ram() { return string(); }

// TODO: Return the user (name) that generated this process
string Process::User() { 
    string statusPath = LinuxParser::kProcDirectory + "/" + to_string(pid) + LinuxParser::kStatusFilename;
    string userId = LinuxParser::GetValue("Uid", statusPath, ':', true);

    std::ifstream filestream(LinuxParser::kPasswordPath);
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

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() { 
    string upTimePath = LinuxParser::kProcDirectory + "/" + to_string(pid) + LinuxParser::kStatFilename;
    string upTimeStr = LinuxParser::SplitFile(upTimePath, 21);
    long upTimeTicks = std::stol(upTimeStr);
    long clockTicks = sysconf(_SC_CLK_TCK);
    long upTimeOffsetSeconds = upTimeTicks / clockTicks;
    System system;
    long upTimeSeconds = system.UpTime() - upTimeOffsetSeconds;
    return upTimeSeconds;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
