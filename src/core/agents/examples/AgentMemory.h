#ifndef _AGENTMEMORY_H_
#define _AGENTMEMORY_H_

#include "../Agent.h"

namespace s21 {
    class AgentMemory : public Agent {
       public:
        AgentMemory();

        void updateMetrics() override;
        std::string toString() override;
        
       private:
        double ram_total_;
        double ram_;
        double hard_volume_;
        int    hard_ops_;
        double hard_throughput_;
    };
}

#endif  // _AGENTMEMORY_H_
