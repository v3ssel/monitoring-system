#ifndef _AGENTNETWORK_H_
#define _AGENTNETWORK_H_

#include "../Agent.h"

namespace s21 {
    class AgentNetwork : public Agent {
       public:
        AgentNetwork();

        void updateMetrics () override;
        std::string toString() override;
        
       private:
        int availability_;
        double inet_throughput_;
    };
}

#endif  // _AGENTNETWORK_H_
