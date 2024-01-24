#ifndef _AGENTEXTRA_H_
#define _AGENTEXTRA_H_

#include "../Agent.h"

namespace s21 {
    class AgentExtra : public Agent {
       public:
        AgentExtra();

        void updateMetrics() override;
        std::string toString() override;
        
       private:
        double cpu_idle_usage_;
        double cpu_user_usage_;
        double cpu_priveleged_usage_;
        double cpu_dpc_usage_;
        double cpu_interrupt_usage_;
        double total_swap_;
        double used_swap_;
        int proc_queue_length_;
        double virtual_mem_volume_;
        double virtual_mem_free_;
        int inodes_;
        double hard_read_time_;
        int system_errors_;
        int user_auths_;
    };
}

#endif  // _AGENTEXTRA_H_
