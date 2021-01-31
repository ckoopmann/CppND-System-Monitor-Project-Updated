#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long activeJiffies = LinuxParser::ActiveJiffies();
    long totalJiffies = LinuxParser::Jiffies();

    float utilization = (float) activeJiffies / (float) totalJiffies;
    return utilization; }

