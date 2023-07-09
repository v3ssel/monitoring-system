#include "agentCPU.h"
#include "commandCaller.h"
#include <sys/sysctl.h>
// #include <format>    //C++20 

extern "C" s21::AgentCPU* create_obj() {
    return new s21::AgentCPU;
}

extern "C" void destroy_obj(s21::AgentCPU* agent) {
    delete agent;
}

// std::string takeValue(std::string command) {
//     FILE* pipe = popen(command.c_str(), "r");
//     if (!pipe) {
//         return "";
//     }

//     char buffer[128];
//     std::string result = "";
//     while(!feof(pipe)) {
//         if(fgets(buffer, 128, pipe) != NULL) {
//             result += buffer;
//         }
//     }
//     pclose(pipe);
//     return result;
// }

void s21::AgentCPU::analyzeSystem() override {
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

    std::string command = "top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1";
    double cpu = stod(takeValue(command));
    std::cout << cpu << std::endl;
    
    command = "top -l 1 | awk ' /^Processes:/{print $2}'";
    int processes = stoi(takeValue(command));
    std::cout << processes << std::endl;

};

std::string s21::AgentCPU::toString() override {
    return "cpu : "s + std::to_string(this->cpu) + " | processes : "s + std::to_string(this->processes);
    // return std::format("cpu : {} | processes : {} ", this->cpu, this->processes);
};
