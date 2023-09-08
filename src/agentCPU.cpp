#include "agentCPU.h"
#include "commandCaller.h"
#include <sys/sysctl.h>
#include <iostream>
#include <chrono>
#include <thread>

extern "C" s21::AgentCPU* create_obj() {
    return new s21::AgentCPU;
}

// extern "C" void destroy_obj(s21::AgentCPU* agent) {
//     delete agent;
// }

s21::AgentCPU::AgentCPU()  {
    is_active = true;
    cpu = 0;
    processes = 0;
    readConfig(".confCPU");
}

void s21::AgentCPU::readConfig(std::string file_name) {
    std::ifstream conf(file_name);
    if (conf.is_open()) {
        std::string line;
        while (std::getline(conf, line)) {
            if (!line.find("update_time") == std::string::npos) {
                std::cout << "dfv" << std::endl;
                update_time = atoi(line.substr(line.find("update_time")).c_str());
                std::cout << update_time << std::endl;
            }
        }
    }
    conf.close();
}

void s21::AgentCPU::updateMetrics() {
    // size_t len;

    // // int req_cpu_load[] {CTL_HW, HW_CPU_LOAD};
    // // sysctl(req_cpu_load, 2, &cpu, &len, NULL, 0);
    // sysctlbyname("kern.cpuload", &cpu, &len, NULL, 0);
    // std::cout << "CPU load: " << cpu << "%" << std::endl;

    // int req_proc[] {CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0};
    // sysctl(req_proc, 4, NULL, &len, NULL, 0);
    // processes = len / sizeof(struct kinfo_proc);
    // std::cout << "Processes: " << processes << std::endl;

    // ////////////////////////////////////////////////////

    // // system("top -l 1 | awk ' /^Processes:/{print $2}'");
    // // system("top -l 2 | awk ' /^CPU/{print $3 + $5}' | tail -1");
    // // system("top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1");
    while (is_active) {
        std::cout << "work" << std::endl;
        std::cout << is_active << std::endl;
        std::string command = "top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1";
        cpu = std::stod(CommandCaller::getInstance().takeValue(command));
        
        command = "top -l 1 | awk ' /^Processes:/{print $2}'";
        processes = std::stoi(CommandCaller::getInstance().takeValue(command));
        // toString();
        std::this_thread::sleep_for(std::chrono::seconds(update_time));
        std::cout << is_active << std::endl;
    };
}

std::string s21::AgentCPU::toString() {
    return "cpu : " + std::to_string(this->cpu) + " | processes : " + std::to_string(this->processes);
};
