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
        double ram_total;
        double ram;
        double hard_volume;
        int    hard_ops;
        double hard_throughput;
    };
}

#endif  // _AGENTMEMORY_H_
