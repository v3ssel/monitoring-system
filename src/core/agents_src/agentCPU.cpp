#include <iostream>

#include "agentCPU.h"
// #include "commandCaller.h"

namespace s21 {
    extern "C" AgentCPU* create_obj() {
        return new AgentCPU;
    }

    AgentCPU::AgentCPU(const std::string& conf_path) {
        cpu_ = 0;
        processes_ = 0;

        this->is_active = true;
        this->name = "AgentCPU";
        this->type = "CPU";
        this->update_time = 3;

        this->comparisons_["cpu"] = Comparisons<double>::is_equal;
        this->comparisons_["processes"] = Comparisons<int>::is_equal;

        this->critical_values_["cpu"] = std::numeric_limits<double>::max();
        this->critical_values_["processes"] = std::numeric_limits<int>::max();

        readConfig(conf_path);
    }

    void AgentCPU::readConfig(const std::string &file_name) {
        std::ifstream conf(file_name);
        if (!conf.is_open()) {
            send_error_(this->name + " error: Could not open configuration file: \"" + file_name + "\".");
            return;
        }

        std::string line;
        while (std::getline(conf, line)) {
            if (line.find("name") == 0) {
                this->name = line.substr(line.find(":") + 1);
            }
            if (line.find("type") == 0) {
                this->type = line.substr(line.find(":") + 1);
            }
            if (line.find("update_time") == 0) {
                this->update_time = std::stoi(line.substr(line.find(":") + 1));
            }
            if (line.find("cpu") == 0) {
                this->SetComparisonsAndCriticals(3, "cpu", line);
            }
            if (line.find("processes") == 0) {
                this->SetComparisonsAndCriticals(9, "processes", line);
            }
        }
    }

    void AgentCPU::updateMetrics() {
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
            // cpu = std::stod(CommandCaller::getInstance().takeValue(command));
            if (comparisons_["cpu"](cpu_, critical_values_["cpu"])) {
                this->send_critical_value_(this->name + " CRITICAL cpu:" + std::to_string(cpu_));
            }

            command = "top -l 1 | awk ' /^Processes:/{print $2}'";
            // processes = std::stoi(CommandCaller::getInstance().takeValue(command));
            if (comparisons_["processes"](processes_, critical_values_["processes"])) {
                this->send_critical_value_(this->name + " CRITICAL: processes:" + std::to_string(processes_));
            }

            // std::this_thread::sleep_for(std::chrono::seconds(update_time));
            std::cout << is_active << std::endl;
        };
    }

    std::string AgentCPU::toString() {
        return "| cpu : " + std::to_string(this->cpu_) + " | processes : " + std::to_string(this->processes_);
    }
}
