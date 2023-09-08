#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__

#include "agent.h"

namespace s21 {
    class AgentCPU : public Agent {
    public:
        AgentCPU();
        void readConfig(std::string file_name) override;
        void updateMetrics() override;
        std::string toString() override;
        
    private:
        double cpu;
        int processes;
    };
}

#endif
