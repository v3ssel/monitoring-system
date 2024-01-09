#ifndef __AGENTCPU_H__
#define __AGENTCPU_H__

#include <unordered_map>
#include <functional>
#include <fstream>
#include "../agent.h"

namespace s21 {
    class AgentCPU : public Agent {
       public:
        AgentCPU(const std::string& conf_path = "./config/.confCPU");
        
        void readConfig(const std::string& file_name) override;
        void updateMetrics() override;
        std::string toString() override;
        
       private:
        double cpu_;
        int processes_;
    };
}

#endif
