#ifndef __AGENTNETWORK_H__
#define __AGENTNETWORK_H__
#include "agentNetwork.h"
#include <chrono>

namespace s21 {
    class AgentNetwork : public Agent {
    public:
        void updateMetrics () override;
        std::string toString() override;
        
    private:
        std::chrono::seconds sleep_time;
        std::string url;
        int availability;
        double inet_throughput;
    };
}

#endif
