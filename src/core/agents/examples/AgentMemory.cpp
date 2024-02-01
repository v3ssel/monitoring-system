#include <limits>

#include "AgentMemory.h"
#include "../AgentConfigReader.h"
#include "../../utils/Comparisons.h"

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

        Agent::metrics_names.push_back("ram_total");
        Agent::metrics_names.push_back("ram");
        Agent::metrics_names.push_back("hard_volume");
        Agent::metrics_names.push_back("hard_ops");
        Agent::metrics_names.push_back("hard_throughput");

        for (auto& metric : Agent::metrics_names) {
            Agent::addCriticalComparison(metric, Comparisons<double>::is_equal, CompareType::IS_EQ);
            Agent::addCriticalValue(metric, std::numeric_limits<double>::max());
        }
    }
    
    void AgentMemory::updateMetrics() {
        // ram_total_ = ...
        if (compare_data_["ram_total"].compare_func(ram_total_, compare_data_["ram_total"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "ram_total", std::to_string(ram_total_));
        }
        
        // ram_ = ...
        if (compare_data_["ram"].compare_func(ram_, compare_data_["ram"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "ram", std::to_string(ram_));
        }
        
        // hard_volume_ = ...
        if (compare_data_["hard_volume"].compare_func(hard_volume_, compare_data_["hard_volume"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "hard_volume", std::to_string(hard_volume_));
        }
        
        // hard_ops_ = ...
        if (compare_data_["hard_ops"].compare_func(hard_ops_, compare_data_["hard_ops"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "hard_ops", std::to_string(hard_ops_));
        }
        
        // hard_throughput_ = ...
        if (compare_data_["hard_throughput"].compare_func(hard_throughput_, compare_data_["hard_throughput"].critical_val)) {
            Agent::observer_->NotifyCritical(this->name, "hard_throughput", std::to_string(hard_throughput_));
        }

        observer_->NotifyResult(this->toString());
    }

    std::string AgentMemory::toString() {
        return "ram_total : " + std::to_string(this->ram_total_) + " | " +
               "ram : " + std::to_string(this->ram_) + " | " +
               "hard_volume : " + std::to_string(this->hard_volume_) + " | " +
               "hard_ops : " + std::to_string(this->hard_ops_) + " | " +
               "hard_throughput : " + std::to_string(this->hard_throughput_);
    }
}