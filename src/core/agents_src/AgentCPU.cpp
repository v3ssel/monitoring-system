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

    AgentCPU::AgentCPU(const std::string& directory) : Agent() {
        cpu_ = 0;
        processes_ = 0;

        Agent::update_time = 3;

        Agent::name = "AgentCPU";
        Agent::type = "CPU";
        Agent::config_name_ = ".conf" + Agent::type;

        Agent::metrics_names_.push_back("cpu");
        Agent::metrics_names_.push_back("processes");

        Agent::comparisons_["cpu"] = Comparisons<double>::is_equal;
        Agent::comparisons_["processes"] = Comparisons<int>::is_equal;

        Agent::critical_values_["cpu"] = std::numeric_limits<double>::max();
        Agent::critical_values_["processes"] = std::numeric_limits<int>::max();

        Agent::readConfig(directory + '/' + Agent::config_name_);
    }

    void AgentCPU::updateMetrics() {
        if (!Agent::observer_) {
            Agent::observer_->NotifyError("ERROR: " + this->name + ": observer is not set.");
            return;
        }
        
        std::random_device rd;
        std::mt19937 gen(rd());
        std::normal_distribution<> d(-10, 10);

        // std::string command = "top -l 2 | awk ' /^CPU/{print 100 - $7}' | tail -1";
        // cpu = std::stod(CommandCaller::getInstance().takeValue(command));
        cpu_ = d(gen);

        if (comparisons_["cpu"](cpu_, critical_values_["cpu"])) {
            Agent::observer_->NotifyCritical("CRITICAL: " + this->name + ": cpu:" + std::to_string(cpu_));
        }

        // command = "top -l 1 | awk ' /^Processes:/{print $2}'";
        // processes = std::stoi(CommandCaller::getInstance().takeValue(command));
        processes_ = d(gen);

        if (comparisons_["processes"](processes_, critical_values_["processes"])) {
            Agent::observer_->NotifyCritical("CRITICAL: " + this->name + ": processes:" + std::to_string(processes_));
        }


        auto awake_time = std::chrono::high_resolution_clock::now() + std::chrono::seconds(update_time);
        while (std::chrono::high_resolution_clock::now() <= awake_time) {
            if (update_time_changed_) {
                update_time_changed_ = false;
                break;
            }
        }
        
        Agent::observer_->NotifyResult(this->toString());
    }

    std::string AgentCPU::toString() {
        return "cpu : " + std::to_string(cpu_) + " | processes : " + std::to_string(processes_);
    }
}
