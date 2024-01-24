#include "agentMemory.h"


namespace s21 {
    extern "C" AgentMemory* create_obj() {
        return new AgentMemory;
    }

    AgentMemory::AgentMemory() {
        ram_total = 0;
        ram = 0;
        hard_volume = 0;
        hard_ops = 0;
        hard_throughput = 0;
    }
    
    void AgentMemory::updateMetrics() {

    }

    std::string AgentMemory::toString() {
        return "total ram: " + std::to_string(this->ram_total) + " | " +
               "ram: " + std::to_string(this->ram) + " | " +
               "hard_volume: " + std::to_string(this->hard_volume) + " | " +
               "hard_ops: " + std::to_string(this->hard_ops) + " | " +
               "hard_throughput: " + std::to_string(this->hard_throughput);
    }
}