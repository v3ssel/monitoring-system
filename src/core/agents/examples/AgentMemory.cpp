#include "AgentMemory.h"
#include "../AgentConfigReader.h"

namespace s21 {
    extern "C" AgentMemory* create_obj() {
        return new AgentMemory;
    }

    AgentMemory::AgentMemory() : Agent() {
        ram_total_ = 0;
        ram_ = 0;
        hard_volume_ = 0;
        hard_ops_ = 0;
        hard_throughput_ = 0;

        Agent::config_reader_ = std::make_unique<AgentConfigReader>(this);
        Agent::name = "AgentMemory";
        Agent::type = "MEM";
        Agent::config_name = ".conf" + Agent::type;

        Agent::metrics_names_.push_back("ram_total");
        Agent::metrics_names_.push_back("ram");
        Agent::metrics_names_.push_back("hard_volume");
        Agent::metrics_names_.push_back("hard_ops");
        Agent::metrics_names_.push_back("hard_throughput");
    }
    
    void AgentMemory::updateMetrics() {
        observer_->NotifyResult(this->toString());
    }

    std::string AgentMemory::toString() {
        return "ram_total: " + std::to_string(this->ram_total_) + " | " +
               "ram: " + std::to_string(this->ram_) + " | " +
               "hard_volume: " + std::to_string(this->hard_volume_) + " | " +
               "hard_ops: " + std::to_string(this->hard_ops_) + " | " +
               "hard_throughput: " + std::to_string(this->hard_throughput_);
    }
}