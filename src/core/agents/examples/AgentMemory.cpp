#include "agentMemory.h"


namespace s21 {
    extern "C" AgentMemory* create_obj() {
        return new AgentMemory;
    }

    AgentMemory::AgentMemory() : Agent() {
        ram_total = 0;
        ram = 0;
        hard_volume = 0;
        hard_ops = 0;
        hard_throughput = 0;

        Agent::name = "AgentMemory";
        Agent::type = "MEM";
        Agent::config_name = ".conf" + Agent::type;
    }
    
    void AgentMemory::updateMetrics() {
        observer_->NotifyResult(this->toString());
    }

    std::string AgentMemory::toString() {
        return "total ram: " + std::to_string(this->ram_total) + " | " +
               "ram: " + std::to_string(this->ram) + " | " +
               "hard_volume: " + std::to_string(this->hard_volume) + " | " +
               "hard_ops: " + std::to_string(this->hard_ops) + " | " +
               "hard_throughput: " + std::to_string(this->hard_throughput);
    }
}