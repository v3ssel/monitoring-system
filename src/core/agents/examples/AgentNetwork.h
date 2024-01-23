#ifndef _AGENTNETWORK_H_
#define _AGENTNETWORK_H_

#include "agent.h"
#include <chrono>

namespace s21 {
    class AgentNetwork : public Agent {
    public:
        AgentNetwork();
        void readConfig(std::string file_name) override;
        void updateMetrics () override;
        std::string toString() override;
        
    private:
        std::string url;
        int availability;
        double inet_throughput;
    };
}

#endif  // _AGENTNETWORK_H_
