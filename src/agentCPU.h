#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__
#include "agent.h"

namespace s21 {
    class AgentCPU : public Agent {
    public:
        void analyzeSystem() override;
        std::string toString() override;
        
    private:
        double cpu;
        int processes;
    };
}

#endif
