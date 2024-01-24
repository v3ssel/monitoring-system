#ifndef _AGENTCPU_H_
#define _AGENTCPU_H_

#include "../Agent.h"

namespace s21 {
    class AgentCPU : public Agent {
       public:
        AgentCPU();
        
        void updateMetrics() override;
        std::string toString() override;
        
       private:
        double cpu_;
        int processes_;
    };
}

#endif  // _AGENTCPU_H_
