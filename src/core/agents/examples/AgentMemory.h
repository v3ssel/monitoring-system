#ifndef _AGENTMEMORY_H_
#define _AGENTMEMORY_H_

#include "../agent.h"

namespace s21 {
    class AgentMemory : public Agent {
       public:
        AgentMemory();

        void setErrorsReceiver(std::function<void(std::string)> errors_receiver) override;
        void readConfig(const std::string& file_name) override;
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
