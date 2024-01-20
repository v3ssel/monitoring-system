#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__

#include "../Agent.h"

namespace s21 {
    class AgentCPU : public Agent {
       public:
        AgentCPU(const std::string& directory = "./config/");
        
        void readConfig(const std::string& file_name) override;
        void updateMetrics() override;
        std::string toString() override;
        
       private:
        double cpu_;
        int processes_;
    };
}

#endif
