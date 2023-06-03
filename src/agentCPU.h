#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__
#include "agent.h"

namespace s21 {
    class AgentCPU : public Agent {
    public:
        void analyzeSystem();
        std::string toString();
        
    private:
        double cpu;
        int processes;
    };
}

#endif
