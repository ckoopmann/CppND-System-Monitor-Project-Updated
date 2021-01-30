#include "processor.h"
#include "linux_parser.h"
#include <unistd.h>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() {
    long activeJiffies = LinuxParser::ActiveJiffies();
    long totalJiffies = LinuxParser::Jiffies();

    int samplingPeriodSeconds = 1;
    usleep(samplingPeriodSeconds * 1000000);

    activeJiffies = LinuxParser::ActiveJiffies() - activeJiffies;
    totalJiffies = LinuxParser::Jiffies() - totalJiffies;

    float utilization = (float) activeJiffies / (float) totalJiffies;
    return utilization; }

