#include "agentCPU.h"
#include <sys/sysctl.h>

extern "C" s21::AgentCPU* create_obj() {
    return new s21::AgentCPU;
}

extern "C" void destroy_obj(s21::AgentCPU* agent) {
    delete agent;
}

void s21::AgentCPU::analyzeSystem() {
    // system("top -l 1 | awk ' /^Processes:/{print $2}'");
    // system("top -l 2 | awk ' /^CPU/{print $3 + $5}' | tail -1");
    // system("top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1");
    size_t len;

    // int req_cpu_load[] {CTL_HW, HW_CPU_LOAD};
    // sysctl(req_cpu_load, 2, &cpu, &len, NULL, 0);
    sysctlbyname("kern.cpuload", &cpu, &len, NULL, 0);
    std::cout << "CPU load: " << cpu << "%" << std::endl;

    int req_proc[] {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
    sysctl(req_proc, 4, NULL, &len, NULL, 0);
    processes = len / sizeof(struct kinfo_proc);
    std::cout << "Processes: " << processes << std::endl;
};

std::string s21::AgentCPU::toString() {
    return "cpu : "s + std::to_string(this->cpu) + " | processes : "s + std::to_string(this->processes);
};
