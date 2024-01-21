#ifndef _AGENTSOBSERVER_H_
#define _AGENTSOBSERVER_H_

#include <string>
#include "Observer.h"

namespace s21 {
    class AgentsObserver : public s21::Observer {
       public:
        virtual void NotifyResult(const std::string& message) = 0;
        // need experiments with Args...
        virtual void NotifyCritical(const std::string& message) = 0;
    };
}

#endif  // _AGENTSOBSERVER_H_
