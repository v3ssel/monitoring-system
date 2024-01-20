#include <iostream>
#include <limits>
#include <chrono>
#include <fstream>

#include <random>

#include "AgentCPU.h"
// #include "commandCaller.h"

namespace s21 {
    extern "C" AgentCPU* create_obj() {
        return new AgentCPU;
    }

    AgentCPU::AgentCPU(const std::string& directory) {
        cpu_ = 0;
        processes_ = 0;

        this->is_active = true;
        this->update_time_changed = false;
        this->update_time = 3;
        this->name = "AgentCPU";
        this->type = "CPU";
        this->config_name = ".conf" + this->type;

        this->comparisons_["cpu"] = Comparisons<double>::is_equal;
        this->comparisons_["processes"] = Comparisons<int>::is_equal;

        this->critical_values_["cpu"] = std::numeric_limits<double>::max();
        this->critical_values_["processes"] = std::numeric_limits<int>::max();

        readConfig(directory + '/' + this->config_name);
    }

    void AgentCPU::readConfig(const std::string &directory) { 
        std::ifstream conf(directory + '/' + this->config_name);
        if (!conf.is_open() && Agent::observer_) {
            Agent::observer_->NotifyError(this->name + " error: Could not open configuration file: \"" + directory + '/' + this->config_name + "\".");
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
                int next_update = std::stoi(line.substr(line.find(":") + 1));

                if (this->update_time != next_update) {
                    update_time_changed = true;
                    this->update_time = next_update;
                }
            }
            if (line.find("cpu") == 0) {
                Agent::SetComparisonsAndCriticals(3, "cpu", line);
            }
            if (line.find("processes") == 0) {
                Agent::SetComparisonsAndCriticals(9, "processes", line);
            }
        }
    }

    void AgentCPU::updateMetrics() {
        if (!Agent::observer_)
            return;

        std::string command = "top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1";
        // cpu = std::stod(CommandCaller::getInstance().takeValue(command));
        if (comparisons_["cpu"](cpu_, critical_values_["cpu"])) {
            Agent::observer_->NotifyCritical(this->name + " CRITICAL cpu:" + std::to_string(cpu_));
        }

        command = "top -l 1 | awk ' /^Processes:/{print $2}'";
        // processes = std::stoi(CommandCaller::getInstance().takeValue(command));
        if (comparisons_["processes"](processes_, critical_values_["processes"])) {
            Agent::observer_->NotifyCritical(this->name + " CRITICAL: processes:" + std::to_string(processes_));
        }


        auto awake_time = std::chrono::high_resolution_clock::now() += std::chrono::seconds(update_time);
        while (std::chrono::high_resolution_clock::now() <= awake_time) {
            if (update_time_changed) {
                update_time_changed = false;
                break;
            }
        }
        
        Agent::observer_->NotifyResult(this->toString());
    }

    std::string AgentCPU::toString() {
        // return "cpu : " + std::to_string(cpu_) + " | processes : " + std::to_string(processes_);
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> d(-10, 10);

        return "cpu : " + std::to_string(d(gen)) + " | processes : " + std::to_string(d(gen));
    }
}
