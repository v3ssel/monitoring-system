#ifndef _AGENTSOBSERVER_H_
#define _AGENTSOBSERVER_H_

#include <string>
#include "Observer.h"

namespace s21 {
    class AgentsObserver : public s21::Observer {
       public:
        virtual ~AgentsObserver() = default;
        virtual void NotifyResult(const std::string& message) = 0;
        virtual void NotifyCritical(const std::string& agent_name, const std::string& metric_name, const std::string& metric_value) = 0;
    };
}

#endif  // _AGENTSOBSERVER_H_
