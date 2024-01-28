#include "AgentExtra.h"

namespace s21 {
    extern "C" AgentExtra* create_obj() {
        return new AgentExtra;
    }

    AgentExtra::AgentExtra() : Agent() {
        cpu_idle_usage_ = 0.0;
        cpu_user_usage_ = 0.0;
        cpu_priveleged_usage_ = 0.0;
        cpu_dpc_usage_ = 0.0;
        cpu_interrupt_usage_ = 0.0;
        total_swap_ = 0.0;
        used_swap_ = 0.0;
        proc_queue_length_ = 0;
        virtual_mem_volume_ = 0.0;
        virtual_mem_free_ = 0.0;
        inodes_ = 0;
        hard_read_time_ = 0.0;
        system_errors_ = 0;
        user_auths_ = 0;

        Agent::name = "AgentExtra";
        Agent::type = "EXT";
        Agent::config_name = ".conf" + Agent::type;
    }
    
    void AgentExtra::updateMetrics() {
        observer_->NotifyResult(this->toString());
    }
    
    std::string AgentExtra::toString() {
        return std::string("extra agent in work");
    }
} // namespace s21
