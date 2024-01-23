#ifndef _AGENTCPU_H_
#define _AGENTCPU_H_

#include "../Agent.h"
#include "../AgentConfigReader.h"

namespace s21 {
    class AgentCPU : public Agent {
       public:
        AgentCPU(const std::string& directory = "./config/");
        
        void updateMetrics() override;
        void setUpdateTime(int new_time) override;
        std::string toString() override;
        
       private:
        double cpu_;
        int processes_;
    };
}

#endif  // _AGENTCPU_H_
